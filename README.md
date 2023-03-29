# TAI_Group_6

To build our program, we have developed a script called build-project.sh. This script can be found in the root of our
project. In order to run the script and compile the program, you need CMake with a version higher or equal to 2.16.

```bash
./build-project.sh
```

After running this script, the binaries for the Copy Model and the Generator should be available inside the /bin folder.
These binaries have the names `cpm` and `gen` respectively.

Run Copy Model:

```bash
./bin/cpm -k 3 -a 0.5 -t 0.45 -i example/chry.txt -o model.txt -s 0
```

Run Generator:

```bash
./bin/cpm_gen -m model.txt -g 500
```

**A list of all the possible arguments for the Copy Model and Generator can be obtained using the `-h` argument. A table with all possible arguments as well as their possible values and descriptions is also available in the Report.**


For the Generator, we must wait for the following message before typing anything. This message indicates that it has
finished loading the Copy Model.

```
-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-[ GENERATOR ]-=-=-==-=-=-=-=-==-=-=-=-=-=-=-=-=-

Write a sentence and the generator will try to complete it.
When you are done, type 'exit'.

-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-==-=-=-=-=-=-=-=-=-
[!] Using Positional Model
[!] Using Non-Interactive Mode
Enter a sequence: 
```
