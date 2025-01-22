"""Display for manual WSPR transmission."""

from encode import wspr_encode

import datetime
import time

def display(symbols):
    for i, symbol in enumerate(symbols):
        if i % 9 == 0:
            print(f'-- {i:3d} ' + '-'*72)
        left = ' ' if symbol & 1 else '#'
        right = ' ' if symbol & 2 else '#'
        print(f'[{left}] [{right}]')

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 4:
        print('Usage: %s callsign locator dbm' % sys.argv[0])
        sys.exit(1)
    callsign = sys.argv[1]
    locator = sys.argv[2]
    dbm = int(sys.argv[3])
    symbols = wspr_encode(callsign, locator, dbm)
    display(symbols)
