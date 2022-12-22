import sys
import subprocess
import statistics
import os

path_to_parallel = sys.argv[1]
path_to_serial = sys.argv[2]

def run_func(func_name, num_threads):
    # Set the OMP_NUM_THREADS environment variable
    env = os.environ.copy()
    env["OMP_NUM_THREADS"] = str(num_threads)

    # Run the function and get the output with maximum priority
    result = subprocess.run([
            "nice", "-n", "-20", 
              func_name, str(num_threads)], 
            stderr=subprocess.PIPE, encoding='utf-8', env=env)

    # Extract the time from the output
    time = float(result.stderr.strip())
    return time

def benchmark(num_threads):
  # Run function A with the given number of threads
  time_a = run_func(path_to_parallel, num_threads)
  # Run function B with a single thread
  time_b = run_func(path_to_serial, 1)
  # Calculate the speedup
  speedup = time_b / time_a
  return speedup

# List of the number of threads to test
thread_counts = [1, 2, 4, 6, 8, 10]

# List of the number of runs to average over
num_runs = [20, 50]

# Save output
with open('results.txt', 'w') as f:
	# Print the table header
	print("Num Runs\t" + "\t".join(f'{str(c)+" CPU":12}' for c in thread_counts),file=f)

	# Print the results in a table
	for n in num_runs:
		# List to store the speedup values for each number of threads
		speedups = []
		stddevs = []
		# Run the benchmark for each number of threads
		for num_threads in thread_counts:
			# List to store the speedup values for each run
			run_speedups = []
			# Run the benchmark multiple times
			for i in range(n):
				speedup = benchmark(num_threads)
				run_speedups.append(speedup)
			# Calculate the average and standard deviation of the speedups
			avg_speedup = statistics.mean(run_speedups)
			std_dev = statistics.stdev(run_speedups)
			speedups.append(avg_speedup)
			stddevs.append(std_dev)
		# Print the results for this number of runs
		print(f"{n:<8}\t" + "\t".join(f"{f'{s:.2f} ({d:.2f})':12}" for s, d in zip(speedups, stddevs)),file=f)
