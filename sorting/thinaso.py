def get_single_number(nums, pos):
    if pos >= 0 and pos < len(nums):
        return nums[pos]
    return 0

def get_number(nums, pos):
    pr = pos+1
    pl = pos-1
    
    while get_single_number(nums, pos) == 0:
        pos = pr
        pr += 1
        if get_single_number(nums, pos) == 0:
            pos = pl
            pl -= 1
    
    n = nums[pos]
    if n != 0:
        nums[pos] = 0
    return n

def sort_numbers(nums):
    import random
    random.seed()
    ret = []
    total = len(nums)
    for i in range(0, total):
        pos = random.randint(0, total-1)
        n = get_number(nums, pos)
        ret.append(n)
        
    return ret

print sort_numbers(range(1, 76))
#print sort_numbers([2, 4, 6, 8, 10, 12])
#print sort_numbers(range(1, 5) + range(6, 10))
