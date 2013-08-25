import sys

f = open('var6.textfont', 'rt')

inchar = False
chars = []
cwidth = 0
for l in f.readlines():
  if not inchar:
    if l.startswith('CHAR'):
      cnum = int(l.split()[1])
      if cnum < 32 or cnum >= 128:
        print "cnum out of range!", cnum, repr(l)
        sys.exit(1)
      cdata = [0, []]
      inchar = True
  else:
    l = l.strip()
    if l != '':
      if cdata[0] == 0:
        cdata[0] = len(l)
      scanline = 0
      bitval = 1
      for c in l:
        if c != '.':
          scanline |= bitval
        bitval <<= 1
      if bitval != (1 << cdata[0]):
        print "bad scanline width?", cnum, bitval
        sys.exit(1)
      cdata[1].append(scanline)
    else:
      if cnum != len(chars) + 32:
        print "cnum don't match??", cnum
        sys.exit(1)
      if len(cdata[1]) != 6:
        print "bad char height?", cnum
        sys.exit(1)
      chars.append(cdata)
      inchar = False

f.close()

f = open('font_var6.h', 'wt')
for c in chars:
  f.write(('    {%d, {' % (c[0],)) + ', '.join(['0x%02X' % sl for sl in c[1]]) + '}},\n')
f.close()


"""
fa = [[0] * 8] * 96
for c in chars:
  newc = []
  for x in c[1]:
    newc.append(
      (0x01 if x & 0x80 != 0 else 0) |
      (0x02 if x & 0x40 != 0 else 0) |
      (0x04 if x & 0x20 != 0 else 0) |
      (0x08 if x & 0x10 != 0 else 0) |
      (0x10 if x & 0x08 != 0 else 0) |
      (0x20 if x & 0x04 != 0 else 0) |
      (0x40 if x & 0x02 != 0 else 0) |
      (0x80 if x & 0x01 != 0 else 0))
  fa[c[0] - 32] = newc


"""
