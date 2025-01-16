"""Display for manual WSPR transmission."""

from encode import wspr_encode

import datetime
import time

def display(symbols, output=False):
    assert len(symbols) == 162, len(symbols)
    baud = 1.4648
    next_k = 20
    while True:
        dt = datetime.datetime.utcnow()
        # t is the offset from the start of the message
        t = (dt.minute % 2) * 60 + dt.second + 1e-6*dt.microsecond - 1
        n = int(t * baud)
        current = '_' if n < 0 or n >= 162 else symbols[n]
        upcoming = symbols[n:n+next_k]
        upcoming = upcoming + [0]*(next_k-len(upcoming))
        bits = [(x & 1, (x >> 1) & 1) for x in upcoming]
        # Display a # if the resistors coming from the VCO voltage should be
        # grounded. Left = bit0, right = bit1.
        print(f"\033[H\033[J", end="")
        for i, (bit0, bit1) in enumerate(bits):
            display = (' ' if bit0 else '#') + '  ' + (' ' if bit1 else '#')
            print(f"{n+i:3d}  [{display:64s}]")
        print(flush=True)
        delay = (n+1)/baud - t
        time.sleep(delay)


if __name__ == '__main__':
    import sys
    if len(sys.argv) != 4:
        print('Usage: %s callsign locator dbm' % sys.argv[0])
        sys.exit(1)
    callsign = sys.argv[1]
    locator = sys.argv[2]
    dbm = int(sys.argv[3])
    symbols = wspr_encode(callsign, locator, dbm)
    display(symbols, output=True)
