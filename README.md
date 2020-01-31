# clas12_NPip



Analysis program for NPip events in the clas12 detector.



##### To Build

```shell
git clone https://github.com/kneupane1/clas12_NPip.git
mkdir -p clas12_NPip/build
cd clas12_NPip/build
cmake ..
make
```

##### To Run

Program takes a beam energy and number of threads as environment variables. Input files are converted using dst2root as part of the [hipo_tools](https://github.com/JeffersonLab/hipo_tools) package.

```shell
NUM_THREADS=8 BEAM_E=10.6041 ./clas12_NPip output.root input_files_*.root
```
