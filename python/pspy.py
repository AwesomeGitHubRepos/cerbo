import psutil

def main():
    for i, proc in enumerate(psutil.process_iter()):
        if i==0: print(proc.__dict__)
        try:
            pinfo = proc.as_dict(attrs=['pid', 'name'])
            if proc.status() == psutil.STATUS_ZOMBIE: print("Zombie:")
        except psutil.NoSuchProcess:
            print("Found NoSuchProcess")
        else:
            print(pinfo)


if __name__ == "__main__":
    main()
