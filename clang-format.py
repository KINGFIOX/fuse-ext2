import os

c_extensions = (".c", ".h")

directories = ["fuse-ext2"]

for directory in directories:
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(c_extensions):
                os.system("clang-format -i -style=file " + root + "/" + file)