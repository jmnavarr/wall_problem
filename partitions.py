
def partitions(n):
   if n:
      for subpart in partitions(n-1):
         yield [1] + subpart
         if subpart and (len(subpart) < 2 or subpart[1] > subpart[0]):
            yield [subpart[0] + 1] + subpart[1:]
   else:
      yield []

#print list(partitions(4))

def brickwall(w,h):
    # generate single brick layer of width w (by recursion)
    def gen_layers(w):
        if w in (0,1,2,3):
            return {0:[], 1:[], 2:[[2]], 3:[[3]]}[w]
        return [(layer + [2]) for layer in gen_layers(w-2)] + \
               [(layer + [3]) for layer in gen_layers(w-3)]
    # precompute info about whether pairs of layers are compatible
    def gen_conflict_mat(layers, nlayers, w):
        # precompute internal brick positions for easy comparison
        def get_internal_positions(layer, w):
            acc = 0; intpos = set()
            for brick in layer:
                acc += brick; intpos.add(acc)
            intpos.remove(w)
            return intpos
        intpos = [get_internal_positions(layer, w) for layer in layers]        
        mat = []
        for i in range(nlayers):
            mat.append([j for j in range(nlayers) \
                              if intpos[i].isdisjoint(intpos[j])])
        return mat
    layers = gen_layers(w)
    nlayers = len(layers)
    mat = gen_conflict_mat(layers, nlayers, w)
    # dynamic programming to recursively compute wall counts
    nwalls = nlayers*[1]
    for i in range(1,h):
        nwalls = [sum(nwalls[k] for k in mat[j]) for j in range(nlayers)]
    return sum(nwalls)

print(brickwall(9,3))   #8
print(brickwall(18,5))  #7958
print(brickwall(32,10)) #806844323190414

