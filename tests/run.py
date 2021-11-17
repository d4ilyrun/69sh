from argparse import ArgumentParser
from pathlib import *
from difflib import unified_diff
from termcolor import colored
import yaml
import subprocess as sp
import os
import os.path

# FILE
def extract_all_yml(dos):
    lis = []
    for root, dirs, files in os.walk(dos):
        for file in files:
            if file.endswith(".yml"):
                path_file = os.path.join(root,file)
                lis.insert(0,path_file)
    return lis

# RUN TEST
def run_shell(args,stdin):
    return sp.run(args,capture_output=True,text=True,input=stdin)

def diff(ref,student):
    ref = ref.splitlines(keepends=True)
    student = student.splitlines(keepends=True)
    return ''.join(unified_diff(ref,student,fromfile="ref",tofile="student"))

def run_one_test(binary,testcase):
    ref = run_shell(["bash","--posix"],testcase["stdin"])
    student = run_shell(binary,testcase["stdin"])

    for check in testcase.get("checks",["stdout","stderr,returncode","has_stderr"]):
        if check == "stdout":
            assert ref.stdout == student.stdout, \
                f"stdout dif:\n{diff(ref.stdout,student.stdout)}"
        elif check == "stderr":
            assert ref.stderr == student.stderr, \
                f"stderr dif:\n{diff(ref.stderr,student.stderr)}"
        elif check == "returncode":
            assert ref.returncode == student.returncode, \
                f"exit code except:{ref.returncode}, got:{student.returncode}\n"
        elif check == "has_stderr" and ref.stderr != "":
            assert student.stderr != "", \
                f"Something was expected on stderr"

def run_yml_test(binary,file_yml,detail):
    nb_test = 0;nb_fail = 0
    pri = []
    with open(file_yml,"r") as tests_files:
            content = yaml.safe_load(tests_files)
    for test in content:
        try:
            run_one_test(binary,test)
        except AssertionError as err:
            nb_fail+= 1
            pri.append(f"[{colored('KO','red')}]" + test["name"] + ('\n' + str(err) if detail else " "))
        else:
            pri.append(f"[{colored('OK','green')}] " + test["name"] )
        nb_test+=1
    name = os.path.dirname(file_yml).split('/')[-2].upper() + ' ' + os.path.dirname(file_yml).split('/')[-1].upper()
    if nb_fail == 0:
        print(f"[{colored('OK','green')}] {name} {nb_test-nb_fail}/{nb_test}")
        return
    else:
        print(f"[{colored('KO','red')}] {name} {nb_test-nb_fail}/{nb_test}")
    for text in pri:
        print('    ' + text)


# MAIN
if __name__ == "__main__":
    parser = ArgumentParser(description="sh TestSuite")
    parser.add_argument("bin",metavar='BIN')
    parser.add_argument("detail",type=int)
    args = parser.parse_args()
    binary = Path(args.bin).absolute()
    
    # TODO:recup la taille du term pour formater le string ;)
    print("----------TEST-SUITE----------")
    list_yml = extract_all_yml('testsuite')
    for file_yml in list_yml:
        run_yml_test(binary,file_yml,args.detail)
    print("------------------------------")
