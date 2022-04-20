import subprocess

#cmd = 'g++ ai_compress.cpp -O3 -march=native -fexcess-precision=fast -funroll-loops -flto -mtune=native -o ai.out'
with open('compile_cmd.txt', 'r') as f:
    cmd = f.read()
o = subprocess.run(cmd, shell=True, encoding='utf-8', timeout=None)
print('------------------compiled------------------')
