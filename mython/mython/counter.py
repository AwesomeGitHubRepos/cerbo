import os
import time

import pyfiglet

#secs = 100

def output(text):
    # insert spacings between letters
    text1 = ""
    for c in text: text1 += c + "  "
    
    pyfiglet.print_figlet(text1)
    
def loop(num_secs):
    base_secs = time.time()
    #print(base_secs)
    while True:
        secs_now = time.time()
        os.system("clear")
        remain_s = int(num_secs - secs_now + base_secs)
        if remain_s <= 0: return
        #print(remain_s)
        #return
        s = remain_s % 60
        mins = remain_s // 60
        out = '{:02}:{:02}'.format(mins, s)
        output(out)

        remain_pc = int( 100.0 * remain_s / num_secs)
        used_pc = 100 - remain_pc
        pc_text =  "{:02}%:{:02}%".format(used_pc, remain_pc)
        output(pc_text)
        
        time.sleep(1)


if __name__ == "__main__":
    #loop(15)
    loop(1155)
    
