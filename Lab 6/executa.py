import os
import time

while True:
    time.sleep(1)
    print("\n")
    for i in range (1, 16) :
        time.sleep(1)
        texto = "./lab6 " + str(i)
        os.system(texto)
