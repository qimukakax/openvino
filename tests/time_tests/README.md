# Time Tests

This test suite contains pipelines, which are executables. The pipelines measure
the time of their execution, both total and partial. A Python runner calls the
pipelines and calcuates the average execution time.

## Prerequisites

To build the time tests, you need to have OpenVINO™ installed or build from source.

## Measure Time

To build and run the tests, open a terminal, set OpenVINO™ environment and run
the commands below:

1. Build tests:
``` bash
source <OpenVINO_install_dir>/setupvars.sh
mkdir build && cd build
cmake .. && make time_tests
```

2. Run test:
``` bash
./scripts/run_timetest.py ../../bin/intel64/Release/timetest_infer -m model.xml -d CPU
```

2. Run several configurations using `pytest`:
``` bash
pytest ./test_runner/test_timetest.py --exe ../../bin/intel64/Release/timetest_infer

# For parse_stat testing:
pytest ./scripts/run_timetest.py
```

