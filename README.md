# Rolling-Window Statistical Container

A custom contiguous container with a rolling window that overwrites old
observations and updates statistics incrementally.

## Overview
This project implements a data structure for maintaining a fixed-size
rolling window over a stream of observations, with statistics (e.g. mean,
variance) updated incrementally rather than recomputed from scratch on each
new observation — designed for efficiency in time-series and streaming
data contexts.

## Features
- Contiguous, fixed-size rolling window container
- Incremental (O(1) amortized) statistic updates on insert/overwrite

## Tech stack
C++
