import readline
import os

while True:
    m = raw_input('ARTIFACTUARY: ')
    f = open('message~', 'wt')
    f.write(m.strip())
    f.close()
    del f
    # make sure file is presented atomically to the artifactuary code
    os.rename('message~', 'message')
