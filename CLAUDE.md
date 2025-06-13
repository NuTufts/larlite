# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Initial Setup
```bash
source config/setup.sh
mkdir build && cd build
cmake -DUSE_PYTHON3:bool=ON ../  # or -DUSE_PYTHON2:bool=ON for Python 2
make install
```

### Common Make Targets
- `make all` - Build all targets
- `make clean` - Clean build files
- `make install` - Install to build/installed/
- `make help` - Show all available targets

### Rebuilding After Changes
```bash
cd build
make install
```

## Testing

### Unit Tests
- **LArUtil Space Charge**: `cd larlite/LArUtil/unittest && python gen_testsample.py && python run_test.py`
- **FhiclLite**: `cd larlite/BasicTool/FhiclLite/test && python test.py`
- **DataFormat**: `cd larlite/DataFormat/mac && python test.py`

### Test Programs
Example I/O test programs are located in `larlite/DataFormat/bin/`:
- `simple_write.cc` - Test writing data
- `simple_read.cc` - Test reading data

## Architecture Overview

larlite is a lightweight data analysis framework for liquid argon TPC detector data, specifically designed for MicroBooNE. It provides simplified access to LArSoft data products without requiring the full LArSoft stack.

### Core Modules

1. **Base** (`larlite/Base/`)
   - Framework constants and configurations
   - Message logging system
   - Analysis framework base classes

2. **LArUtil** (`larlite/LArUtil/`)
   - Detector geometry handling
   - Space charge corrections
   - Detector properties and time service
   - MicroBooNE-specific configurations

3. **DataFormat** (`larlite/DataFormat/`)
   - Data structures for physics objects (hits, tracks, showers, clusters)
   - Monte Carlo truth information
   - Storage management system (`storage_manager`)
   - Event associations between different data products

4. **BasicTool** (`larlite/BasicTool/`)
   - FhiclLite: Configuration file parser for parameter management

5. **Analysis** (`larlite/Analysis/`)
   - Base classes for user analysis modules
   - Event processor framework

### Key Design Patterns

- **Event-based processing**: Data is organized by events with a storage_manager handling I/O
- **ROOT integration**: All classes have ROOT dictionaries for persistence and PyROOT bindings
- **CMake build pattern**: Each module defines headers, sources, creates shared library with ROOT dictionary
- **Header convention**: All headers should be included with `larlite/` prefix (e.g., `#include "larlite/Base/DataFormatConstants.h"`)

### Development Tools

- `larlite-config`: Get configuration info (includes, libs)
- `eventdump.py <file> [max_events]`: Dump event contents
- `larlite`: Interactive ROOT session with larlite loaded
- Code generation tools: `llgen_package`, `llgen_class_anaunit`, etc.

### Python Usage Pattern
```python
from larlite import larlite as fmwk
mgr = fmwk.storage_manager()
mgr.set_io_mode(mgr.kREAD)
mgr.add_in_filename("input.root")
mgr.open()
# Process events...
```

### Important Notes
- This codebase requires ROOT 6 and supports both Python 2 and 3
- The C++ standard (11/14/17) is auto-detected based on ROOT version
- No formal linting tools are configured in the build system
- The project focuses on MicroBooNE detector analysis but can be adapted for other LAr TPCs