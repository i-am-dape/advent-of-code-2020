arr = [int(line.rstrip()) for line in open('input_raw.txt', 'r').readlines()]
arr.sort()
print(arr)
arr.append(arr[-1]+3)
print(arr[-1])

memo = {0: 1}

for r in arr:
  print(r, memo)
  memo[r] = memo.get(r-3,0) + memo.get(r-2,0) + memo.get(r-1,0)
  print(memo)
  

print(memo[arr[-1]])
