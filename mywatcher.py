#!/usr/bin/env python3
"""
mywatcher - Watch files and run commands on changes.

A simple file watcher utility that monitors specified file patterns
and automatically re-runs a command when changes are detected.
"""
import fnmatch
import os
import subprocess
import sys
import time
from typing import List, Optional

from watchdog.events import FileSystemEventHandler
from watchdog.observers import Observer
from watchdog.observers.polling import PollingObserver


class CommandWatcher(FileSystemEventHandler):
    """
    File system event handler that runs a command when watched files change.
    
    Attributes:
        command: The shell command to execute on file changes.
        watch_patterns: List of glob patterns to watch.
        debounce_seconds: Minimum time between command executions.
    """
    
    def __init__(self, command: str, watch_patterns: List[str], debounce_seconds: float = 0.5):
        super().__init__()
        self.command = command
        self.watch_patterns = watch_patterns
        self.debounce_seconds = debounce_seconds
        self._process: Optional[subprocess.Popen] = None
        self._last_run: float = 0

    def _matches_pattern(self, path: str) -> bool:
        """Check if file matches any of the glob patterns."""
        filename = os.path.basename(path)
        
        try:
            rel_path = os.path.relpath(path)
        except ValueError:
            # On Windows, relpath can fail across drives
            rel_path = path
        
        for pattern in self.watch_patterns:
            pattern_basename = os.path.basename(pattern)
            
            # Match against filename
            if fnmatch.fnmatch(filename, pattern_basename):
                return True
            
            # Match against relative path
            if fnmatch.fnmatch(rel_path, pattern):
                return True
        
        return False

    def _handle_file_event(self, event) -> None:
        """Common handler for file modification and creation events."""
        if event.is_directory:
            return

        if not self._matches_pattern(event.src_path):
            return

        # Debounce rapid changes
        current_time = time.time()
        if current_time - self._last_run < self.debounce_seconds:
            return
        self._last_run = current_time

        print(f"\n[File changed: {event.src_path}]")
        self.run_command()

    def on_modified(self, event) -> None:
        """Handle file modification events."""
        self._handle_file_event(event)

    def on_created(self, event) -> None:
        """Handle file creation events."""
        self._handle_file_event(event)

    def _stop_current_process(self) -> None:
        """Stop the currently running command and all its children."""
        if self._process is None or self._process.poll() is not None:
            return
        
        print("[Stopping previous command...]")
        
        # On Windows, shell=True creates cmd.exe which spawns the actual process.
        # terminate() only kills cmd.exe, leaving the child orphaned.
        # Use taskkill /F /T to kill the entire process tree.
        if sys.platform == 'win32':
            subprocess.run(
                ['taskkill', '/F', '/T', '/PID', str(self._process.pid)],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
        else:
            # On Linux/macOS, kill the entire process group
            import signal
            try:
                os.killpg(self._process.pid, signal.SIGTERM)
            except ProcessLookupError:
                pass  # Process already dead
            try:
                self._process.wait(timeout=2)
            except subprocess.TimeoutExpired:
                try:
                    os.killpg(self._process.pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
        
        self._process.wait()

    def run_command(self) -> None:
        """Run the command, stopping any previous instance first."""
        self._stop_current_process()
        
        print(f"[Running: {self.command}]")
        print("-" * 50)

        # Let command print directly to terminal to preserve colors/formatting
        # Don't wait() - let it run in background so file events can still be processed
        # On Linux/macOS, start_new_session creates a new process group for killpg()
        self._process = subprocess.Popen(
            self.command,
            shell=True,
            start_new_session=(sys.platform != 'win32'),
        )

    def stop(self) -> None:
        """Clean up when stopping the watcher."""
        self._stop_current_process()


def print_help() -> None:
    """Display help information."""
    help_text = """\
mywatcher - Watch files and run commands on changes

USAGE:
    mywatcher [--help] [--polling] <glob_pattern> [<glob_pattern>...] -- <command>
    mywatcher [--help] [--polling] <glob_pattern> [<glob_pattern>...] <command>

ARGUMENTS:
    glob_pattern    One or more glob patterns for files to watch (e.g., "*.c", "*.h")
    command         The command to run when files change

OPTIONS:
    -h, --help      Show this help message and exit
    --polling       Use polling mode (100ms interval) instead of native filesystem events.
                    Useful for WSL environments where watchdog can't detect changes.

EXAMPLES:
    # Watch all .c files and compile when they change
    mywatcher "*.c" -- "clear && gcc main.c -o main && ./main"

    # Watch multiple file patterns (using -- separator)
    mywatcher "*.c" "*.h" -- "clear && make"

    # Watch all Python files and run tests
    mywatcher "*.py" -- pytest

    # Legacy mode (patterns auto-detected by * or ? characters)
    mywatcher "*.c" "clear && make"

NOTES:
    - Use '--' to explicitly separate patterns from the command
    - Press Ctrl+C to stop the watcher
    - The command will auto-restart if it's still running when files change
"""
    print(help_text)


def parse_arguments(args: List[str]) -> tuple[List[str], str, bool]:
    """
    Parse command line arguments into patterns, command, and polling flag.
    
    Supports two modes:
    1. Explicit separator: patterns -- command
    2. Auto-detection: patterns containing * or ? are treated as patterns
    
    Returns:
        Tuple of (patterns, command, use_polling)
    
    Raises:
        ValueError: If arguments are invalid
    """
    if not args:
        raise ValueError("No arguments provided")
    
    # Check for --polling flag
    use_polling = '--polling' in args
    if use_polling:
        args = [arg for arg in args if arg != '--polling']
    
    if not args:
        raise ValueError("No arguments provided after removing flags")
    
    # Check for explicit separator
    if '--' in args:
        separator_idx = args.index('--')
        patterns = args[:separator_idx]
        command_parts = args[separator_idx + 1:]
        
        if not patterns:
            raise ValueError("No patterns specified before '--'")
        if not command_parts:
            raise ValueError("No command specified after '--'")
        
        return patterns, ' '.join(command_parts), use_polling
    
    # Legacy mode: auto-detect patterns by glob characters
    patterns: List[str] = []
    command_parts: List[str] = []
    in_command = False

    for arg in args:
        if not in_command and ('*' in arg or '?' in arg):
            patterns.append(arg)
        else:
            in_command = True
            command_parts.append(arg)

    # Fallback: first arg is pattern, rest is command
    if not patterns:
        patterns = [args[0]]
        command_parts = args[1:]

    if not command_parts:
        raise ValueError("No command specified")

    return patterns, ' '.join(command_parts), use_polling


def main() -> None:
    """Main entry point for the file watcher."""
    # Check for help flag
    if '--help' in sys.argv or '-h' in sys.argv:
        print_help()
        sys.exit(0)

    if len(sys.argv) < 3:
        print("Usage: mywatcher <glob_pattern> [<glob_pattern>...] -- <command>")
        print("Example: mywatcher '*.c' -- 'clear && gcc main.c -o main && ./main'")
        print("\nLegacy mode (auto-detect patterns):")
        print("Example: mywatcher '*.c' '*.h' 'clear && make'")
        print("\nUse --help for more information.")
        sys.exit(1)

    try:
        patterns, command, use_polling = parse_arguments(sys.argv[1:])
    except ValueError as e:
        print(f"Error: {e}")
        sys.exit(1)

    print(f"Watching patterns: {patterns}")
    print(f"Command: {command}")
    if use_polling:
        print("Mode: Polling (100ms interval)")
    print("Press Ctrl+C to stop watching")
    print("-" * 50)

    event_handler = CommandWatcher(command, patterns)
    
    # Use PollingObserver for WSL compatibility if requested
    if use_polling:
        observer = PollingObserver(timeout=0.1)  # 100ms polling interval
    else:
        observer = Observer()
    
    observer.schedule(event_handler, path='.', recursive=True)

    try:
        observer.start()
        event_handler.run_command()  # Run initially

        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n[Stopping watcher...]")
        observer.stop()
        event_handler.stop()
    finally:
        observer.join()


if __name__ == "__main__":
    main()
