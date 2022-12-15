def taskReplace(array):
    for i in range(len(array) // 2):
        array[i], array[len(array)-1-i] = array[len(array)-1-i], array[i]
    return array


example = [i for i in range(12)]
print(taskReplace(example))
