import time
from multiprocessing import Pool

# ─── Merge Helper ─────────────────────────────────────────
def merge(left, right):
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result

# ─── Sequential Merge Sort ────────────────────────────────
def merge_sort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    return merge(left, right)

# ─── Parallel Merge Sort ──────────────────────────────────
def parallel_merge_sort(arr):
    if len(arr) <= 1:
        return arr
    if len(arr) < 1000:       # avoid overhead for small arrays
        return merge_sort(arr)
    mid = len(arr) // 2
    with Pool(2) as p:
        left, right = p.map(parallel_merge_sort, [arr[:mid], arr[mid:]])
    return merge(left, right)

# ─── Take Input ───────────────────────────────────────────
def take_input():
    print("=" * 45)
    print("   Parallel Merge Sort — Assignment 2(B)")
    print("=" * 45)
    n = int(input("Enter number of elements: "))
    arr = list(map(int, input("Enter elements (space separated): ").split()))
    if len(arr) != n:
        print(f"Warning: You entered {len(arr)} elements, expected {n}. Using what you entered.")
    return arr

# ─── Main ─────────────────────────────────────────────────
if __name__ == "__main__":
    arr = take_input()

    arr_seq = arr.copy()
    arr_par = arr.copy()

    print("\nOriginal Array:", arr)

    # Sequential Merge Sort
    start = time.time()
    sorted_seq = merge_sort(arr_seq)
    end = time.time()
    seq_time = end - start
    print("\n--- Sequential Merge Sort ---")
    print("Sorted Array:", sorted_seq)
    print(f"Time taken  : {seq_time:.6f} seconds")

    # Parallel Merge Sort
    start = time.time()
    sorted_par = parallel_merge_sort(arr_par)
    end = time.time()
    par_time = end - start
    print("\n--- Parallel Merge Sort ---")
    print("Sorted Array:", sorted_par)
    print(f"Time taken  : {par_time:.6f} seconds")

    # Performance Comparison
    print("\n--- Performance Comparison ---")
    print(f"Sequential Time : {seq_time:.6f} sec")
    print(f"Parallel Time   : {par_time:.6f} sec")
    if par_time > 0:
        speedup = seq_time / par_time
        print(f"Speedup         : {speedup:.2f}x")
    print(f"Both outputs match: {sorted_seq == sorted_par}")