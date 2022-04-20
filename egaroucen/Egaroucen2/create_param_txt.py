from tqdm import trange

n_phases = 4
n_patterns = 11
max_evaluate_idx = 59049
max_canput = 30
max_surround = 50
n_add_dense1 = 8
n_all_input = 19
pattern_size = [8, 8, 8, 5, 6, 7, 8, 10, 10, 10, 10]

with open('param.txt', 'r') as f:
    params = f.read().splitlines()
param = ','.join(params)

with open('src/evaluate_before.hpp', 'r') as f:
    evaluate = f.read()
evaluate = evaluate.replace('PARAM_REPLACE_HERE', param)

with open('src/evaluate.hpp', 'w') as f:
    f.write(evaluate)