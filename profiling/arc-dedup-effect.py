#! /usr/bin/env python3

# This script is used to evaluate the effect of deduplication pass on the performance of arcilator.
#
# Related options:
#   1. --dedup                                              - Deduplicate arcs
#   2. --inline                                             - Inline arcs
#
# Besides, we will also look into the synergy with LLVM PGO (profile-guided optimization).

import argparse
import subprocess
import tempfile
from pathlib import Path


def build(design: str, config: str, build_dir: Path, **kwargs) -> Path:
    """Build the design. Return the path to the binary."""
    # Get the absolute path to the rocket directory
    rocket_dir = Path.cwd() / 'rocket'

    # Create build directory if it doesn't exist
    build_dir.mkdir(parents=True, exist_ok=True)

    # Create build log file
    build_log = build_dir / 'build.log'

    try:
        # Prepare the command
        cmd = [
            'make', '-C',
            rocket_dir.as_posix(), 'build', f'CONFIG={config}', f'BUILD_DIR={build_dir.as_posix()}',
            *(f'{k.upper()}={str(v)}' for k, v in kwargs.items())
        ]

        # Run make with the specified design and config, redirecting both stdout and stderr to the log file
        with open(build_log, 'w') as f:
            # Write the command line to the log
            f.write("\n\n" + "-" * 80 + "\n")
            f.write(f"Command: {' '.join(cmd)}\n")

            result = subprocess.run(
                cmd,
                stdout=f,  # Redirect stdout to the file
                stderr=subprocess.STDOUT,  # Redirect stderr to stdout (which goes to the file)
                text=True,  # Use text mode for easier reading
                check=True  # Raise CalledProcessError if the command fails
            )

        print(f"Build log saved to: {build_log.as_posix()}")

    except subprocess.CalledProcessError as e:
        print(f"Build failed. Check the build log at: {build_log.as_posix()}")
        raise

    # Return the path to the built binary
    return build_dir / f'{design}-main'


def run(binary: Path, benchmark: Path, output_dir: Path) -> None:
    """Run the design. Profile it with perf."""
    try:
        # Create output directory if it doesn't exist
        output_dir.mkdir(parents=True, exist_ok=True)

        # Create a meaningful output file name based on the binary name
        perf_output = output_dir / f"{binary.stem}.perf"

        # Run perf and save output to the file
        result = subprocess.run([
            'perf', 'stat', '-r', '10', '-o',
            perf_output.as_posix(), '-S', 'bash', '-c', f'{binary.as_posix()} --arcs {benchmark.as_posix()}'
        ],
                                capture_output=True,
                                text=True,
                                check=True)

        # Print the perf output
        print(f"Perf Statistics saved to: {perf_output}")
        print(perf_output.read_text())

    except subprocess.CalledProcessError as e:
        print(e.stderr)
        raise


def parse_args():
    """Parse the command line arguments."""
    parser = argparse.ArgumentParser()
    # Build options
    parser.add_argument('--design', type=str, default='rocket', help='The design to build.')
    parser.add_argument('--config', type=str, default='large-master', help='The configuration to use.')
    # Run options
    parser.add_argument('--simulator', type=str, default='arcilator', help='The simulator to run.')
    parser.add_argument('--benchmark',
                        type=Path,
                        default=Path.cwd() / 'benchmarks' / 'dhrystone.riscv',
                        help='The benchmark to run.')
    return parser.parse_args()


def main():
    args = parse_args()

    # Build the design with default configuration
    dedup_build_dir = Path.cwd() / 'build' / 'dedup'
    dedup_binary = build(args.design,
                         args.config,
                         dedup_build_dir,
                         arcilator_args='--dedup=true --mlir-timing --print-debug-info --mlir-pass-statistics')

    # Run the benchmark with default configuration
    print("Running the benchmark with deduplication pass enabled...")
    run(dedup_binary, args.benchmark, dedup_build_dir)

    print("-" * 100)

    # Build the design with deduplication pass disabled
    nodedup_build_dir = Path.cwd() / 'build' / 'nodedup'
    nodedup_binary = build(args.design,
                           args.config,
                           nodedup_build_dir,
                           arcilator_args='--dedup=false --mlir-timing --print-debug-info --mlir-pass-statistics')

    # Run the benchmark with deduplication pass disabled
    print("Running the benchmark with deduplication pass disabled...")
    run(nodedup_binary, args.benchmark, nodedup_build_dir)


if __name__ == '__main__':
    main()
