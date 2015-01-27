#! /usr/bin/env python3

import site
site.addsitedir("/home/tiraka/runner-dependencies/lib/python3.3/site-packages")

import re
import sys
import os
import signal
import string
import tempfile
import shutil
import subprocess
#import threading
import difflib
import getpass

from mako.template import Template

PYTHONLOCATION="/usr/bin/python3"

#code under test
if len(sys.argv) == 3:
    code = [sys.argv[1], sys.argv[2]]
elif len(sys.argv) == 2:
    code = [None, sys.argv[1]]
else:
    print("Please supply datastructure file(s)", file=sys.stderr)
    print("In case of cpp: run_tests.py datastructure.hh datastructure.cc", file=sys.stderr)
    print("In case of python: run_tests.py datastructure.py", file=sys.stderr)
    sys.exit(1)

tests = ["test_public", "test_empty_ds", "test_same_birth_years",
         "test_oldest", "test_youngest", "test_more_personnel"]

test_names = [
	"Compilation:",
	"Compilation without warnings:",
	"Public test:",
	"Empty data structure:",
	"Same birth years",
	"Oldest",
	"Youngest",
	"More personnel",
	"Efficiency:"
]

test_results = [
	"Compile was success",
	"0/100",
	"0/100",
	"0/100",
	"0/100",
	"0/100",
	"0/100",
	"0/100",
	"0/100"	
]

test_average = 0

home = "/home/tiraka/Harjoitustyot_public/hw1_sortedredshirt/"
julkinen = "./"

def sigterm_handler(signum, frame):
    os.killpg(0, signal.SIGTERM)
    sys.exit()

def main(): 
    global test_average
    signal.signal(signal.SIGTERM, sigterm_handler)
    with tempfile.TemporaryDirectory() as tempdir:
        sut = tempdir + "/"
        python_sut = os.path.join(sut, "python_model")
        student = os.getcwd()+"/"

        if code[0] is None or code[0][:1] == '/':
            hdr = code[0]
        else:
            hdr = student+code[0]
        if code[1][:1] == '/':
            src = code[1]
        else:
            src = student+code[1]

        if src.endswith(".py"):
            cpp = False
        else:
            cpp = True


        tests_in = [os.path.join(julkinen, "{0:s}.in".format(test)) for test in tests]

        tests_out = [os.path.join(julkinen, "{0:s}.out".format(test)) for test in tests]

        graphs_in = os.path.join(julkinen, "test_graphs.in")
        if cpp:
            eff_in = os.path.join(julkinen, "test_eff_cpp.in")
        else:
            eff_in = os.path.join(julkinen, "test_eff_python.in")

        #starting the testing process
        if cpp:
            shutil.copy(os.path.join(home,"Makefile_cpp"), os.path.join(sut,'Makefile'))
            shutil.copy(os.path.join(home,"main.cc"), sut)
            eff_home = home
        else:
            shutil.copy(os.path.join(home,"main.py"), sut)
            os.mkdir(python_sut)
            shutil.copy(os.path.join(home,"main.py"), python_sut)
            shutil.copy(os.path.join(home,"datastructure.pyc"), python_sut)
            eff_home = python_sut

        if hdr is not None:
            shutil.copy(hdr, sut)
        shutil.copy(src, sut)

        points = []
        if cpp:
            retval = compile(sut)
#            mainprog = "prog"
            effprog = mainprog = "eff"
            python = None
            #if the program did not compile we will return
            if retval != 0:
                test_results[0] = "Compile failed"
                renderTemplate(0, cpp, sut)
                return 1
            else:
                warnings = checkWarnings(sut)
                test_results[1] = str(warnings) + "/100"
                test_average += warnings
        else:
            mainprog = effprog = "main.py"
            python = PYTHONLOCATION
            test_results[0] = test_results[1] = False

        printLine()
        print("            Running tests.")
        print("This may take few minutes, please wait.")
        printLine()

        #running tests
        diffTest(python, mainprog, tests_in, tests_out, sut)


        # +1 for warnings
        if cpp:
            average = test_average / (len(tests_in) + 1)
        else:
            average = test_average / len(tests_in)

        graphTest(python, effprog, graphs_in, sut, julkinen)

        eff_points = effTest(python, effprog, eff_in, sut, eff_home)
        test_results[8] = str(eff_points) + "/100"


        #Printing the final results of the tests
        renderTemplate(average, cpp, sut)

        return 0

def renderDiffTemplate(index, sut, results_dir):
    outfilename = os.path.join(sut, "test{0:d}.out".format(index))
    if os.path.exists(outfilename):
        with open(outfilename, 'r', encoding='utf-8') as filu:
            out_actualdata = filu.read()
#            out_actualdata = re.sub( '( |\t)+', ' ', out_actualdata)
    else:
        out_actualdata = "Error during test execution"

    with open(os.path.join(julkinen, tests[index] + ".out"), 'r', encoding='utf-8') as filu:
        out_data = filu.read()
#        out_data = re.sub( '( |\t)+', ' ', out_data)
    with open(os.path.join(julkinen, tests[index] + ".in"), 'r', encoding='utf-8') as filu:
        in_data = filu.read()

    table = difflib.unified_diff(out_data.splitlines(), out_actualdata.splitlines(), fromfile="expected output", tofile="your output")
    table = "\n".join(table)
    
    tmplData = {}
    tmplData['TestActualOutput'] = out_actualdata
    tmplData['TestName'] = test_names[index + 2]
    tmplData['TestInput'] = in_data
    tmplData['TestOutput'] = out_data
    tmplData['TestDifferences'] = table
    mytemplate = Template(filename=os.path.join(julkinen,"diff_template.html"))
    resultFile = open(os.path.join(results_dir, "test{0:d}.html".format(index)), 'w')
    resultFile.write(mytemplate.render(**tmplData))
    

def renderTemplate(average, cpp=True, sut=None):
	
    printLine()
    results = zip(test_names, test_results)

    tmplData = {}
    tmplData['cpp'] = cpp

    results_dir = "../results"

    if not os.path.isdir(results_dir):
        os.mkdir(results_dir)
    results_dir = os.path.join(results_dir, "hw1")
    if not os.path.isdir(results_dir):
        os.mkdir(results_dir)

    for i, (name, result) in enumerate(results):
        tmplData["Test" + str(i) + "Name"] = name
        tmplData["Test" + str(i) + "Result"] = result
        if i < 6:
            renderDiffTemplate(i, sut, results_dir)

    tmplData['TestAverage'] = 'Test Average'
    tmplData['TestAverageResult'] = str(average) + "/100"

    mytemplate = Template(filename=os.path.join(julkinen,"template.html"))
    
    resultFile = open(os.path.join(results_dir, "index.html"), 'w')
    resultFile.write(mytemplate.render(**tmplData));
    for image_file in ["sort.png", "add.png", "del.png", "old.png", "young.png"]:
        if os.path.exists(image_file):
            shutil.copy(image_file, results_dir)
            os.remove(image_file)
    shutil.copy("template.css", "../results/hw1")

    printScore(test_results, average, cpp)
    print("Testing done, results are available in:")
    print("../results/hw1/")

    printLine()

    print("To view results in browser,")
    print("you can copy the results directory to your public_html")
    print("with following commands:")
	
    print("If the directory doesn't exist:")
    print("mkdir ~/public_html")
    print("chmod o+rx ~/public_html")
    print("")

    print("cp -r ../results/ ~/public_html/")
    print("chmod -R o+rX ~/public_html/results")
    print("chmod -R o+r ~/public_html/results/")
    print("")
    print("Alternatively you can run make command publish:")
    print("make publish")
    printLine()
	
    print("Then results are viewable in:")
    print("http://www.cs.tut.fi/~{0:s}/results/hw1".format(getpass.getuser()))

    printLine()

def printLine():
    print("-"*42)

def compile(sut):
    warnings = open(sut+"warnings", 'w')
    p = subprocess.Popen(["make", "-C", sut], stderr=warnings)
    retval = p.wait()
    return retval

def checkWarnings(sut):
    sum = 0
    with open(sut+"warnings", 'r') as file:
        for line in file:
            if "warnings generated" in line:
                t = line.split()
                t = int(t[0])
                #if there is more than 20 warnings the score will be 0
                if t > 20:
                    return 0
                else:
                    sum = sum + t
    if sum > 20:
        return 0
    elif sum == 0:
        return 100
    else:
        score = int(100-((sum*100)/20))
        return score


def printScore(points, average, cpp):
    score = 0
    tests = 0
    printLine()
    for i in range(len(test_names)):
        if not cpp and i < 2:
            continue

        if points[i] is not False:
            print("  "+test_names[i]+" "+points[i])
            #score = score + points[i]
            #tests = tests+1
        else:
            print("  "+test_names[i]+" Discarded from the final score.")

    #score = score/tests
    printLine()
    print("Final score: "+str(int(average))+" / 100")
    printLine()

#functional tests
def diffTest(python, test_program, inputs, outputs, sut):
    print("Running functionality tests")
    test_program = os.path.join(sut, test_program)
    global test_average
    for i, input in enumerate(inputs):
        if python is not None:
            command = [python, test_program]
        else:
            command = [test_program]
#		command = sut+"tirakaprog"
        out_file = os.path.join(sut, "test{0:d}.out".format(i))
        test = Test(command, inputs[i], out_file, False)
        timeouted = test.run(timeout=30)
        
        if timeouted == -15:
            print(test_names[i+2]+"Test aborted due to timeout.")
            print("Continuing tests, please wait.")
            test_results[i + 2] = str(0) + "/100"
        elif timeouted != 0:
            print(test_names[i+2]+"Test aborted due to runtime error.")
            print("Continuing tests, please wait.")
            test_results[i + 2] = str(0) + "/100"
        else:
            with open(out_file, 'r', encoding='utf-8') as filu:                
                expected_output = filu.read()
#                expected_output = re.sub( '( |\t)+', ' ', expected_output)
            with open(outputs[i], 'r', encoding='utf-8') as filu:
                output = filu.read()
#                output = re.sub( '( |\t)+', ' ', expected_output)

            s = difflib.SequenceMatcher(a=expected_output,
                                        b=output)
            difference = 100 * s.ratio()
            """
            diff_file = sut+"diff"+str(i)
            diff = open(diff_file, 'w')
            p = subprocess.Popen(["diff","-y","--suppress-common-lines",outputs[i],out_file], stdout = diff)
            p.wait()
            diff.flush()

            wc = subprocess.Popen(["wc", "-l", diff_file], stdout=subprocess.PIPE)
            tmp = wc.stdout.read()
            diff_lines = [int(s) for s in tmp.split() if s.isdigit()]
            wc = subprocess.Popen(["wc", "-l", outputs[i]], stdout=subprocess.PIPE)
            tmp = wc.stdout.read()
            output_lines = [int(s) for s in tmp.split() if s.isdigit()]
            difference = 100-(float(diff_lines[0])*100 / float(output_lines[0]))
            """			
            if difference < 0:
                test_results[i + 2] = str(0) + "/100"
            else:
                test_results[i + 2] = str(int(difference)) + "/100"
                test_average += int(difference)

    
def graphTest(python, test_program, input, sut, julkinen):
    print("Running graph generation tests")
    sut_eff = os.path.join(sut, test_program)
    if python is None:
        command = [sut_eff]
    else:
        command = [python, sut_eff]
    out_file = "/dev/null"
#	time_sut = sut+"time"
    test = Test(command, input, out_file, None)

    timeouted = test.run(timeout=420)
    if timeouted == -15:
        print("Graph generation aborted due to timeout.")
    DEVNULL = open(os.devnull, 'wb')
    subprocess.Popen(['gnuplot', julkinen + "plot.gpl"], stdout=DEVNULL, stderr=DEVNULL)
    return 0


def effTest(python, test_program, input, sut, home):
    print("Running efficiency tests")
    if python is None:
        model_eff = os.path.join(home, "tiraka{0:s}".format(test_program))
    else:
        model_eff = os.path.join(home, test_program)

    if not os.path.exists(model_eff):
        print("Test aborted")
        return 0

    sut_eff = os.path.join(sut, test_program)
    out_file = os.path.join(sut,"effout")

    sum_model = 0.0
    sum_sut = 0.0

    measurements = 7


    if python is not None:
        command_eff = [python, sut_eff]
        command_model = [python, model_eff]
    else:
        command_eff = [sut_eff]
        command_model = [model_eff]

    for i in range(measurements):
        time_sut = os.path.join(sut, "time{0:d}".format(i))
        test = Test(command_eff, input, out_file, time_sut)
        timeouted = test.run(timeout=45)
        if timeouted == -15:
            print("Efficiency: test aborted due to timeout.")
            print("Continuing tests, please wait.")
            return 0
        elif timeouted != 0:
            print("Efficiency: test aborted due to runtime error.")
            print("Continuing tests, please wait.")
            return 0
        time_model = os.path.join(sut, "timeM{0:d}".format(i))
        print(".", end="", flush=True)
        test = Test(command_model, input, out_file, time_model)
        timeouted = test.run(timeout=45)
        if timeouted != 0:
            printLine()
            print("Could not finish efficiency test due to problems in model program.")
            print("Test will be discarded from the final score")
            print("If you get this error please send an email to course staff")
            print("and try to run the script again later.")
            printLine()
            return False

        latest_time = parseTime2(time_sut)
        if latest_time is None:
            return 0
        sum_sut += latest_time
        latest_time = parseTime2(time_model)
        if latest_time is None:
            return 0
        sum_model += latest_time

        print(".", end="", flush=True)
    print("", flush=True)

    avg_model = sum_model/measurements
    avg_sut = sum_sut/measurements
    #if the tested program was faster than model we know that score is 100
    #else we can determine it by calculating how much faster the model was
    if avg_sut <= avg_model:
        score = 100
    else:
        score = int((avg_model/avg_sut)*100)
    return score

def parseTime2(input):
    with open(input, 'r') as input_file:
        try:
            input_time = float(input_file.readline().strip())
        except ValueError:
            return None
        return input_time

#helper function for parsing file that contains output from
#bash time
def parseTime(input):
    with open(input, 'r') as file:
        for line in file:
            if "user" in line:
                #splitting "sys" and "0mS.MMMs" to two variables in array
                t = line.split()
                #converting the 0mS.MMMs" to format "S.MMM" and to float
                t = float(t[1][2:-1])
                return t

#test object which provides timeout
class Test(object):
    def __init__(self, command, in_file, out_file, err_file):
        self.command = command
        self.in_file = in_file
        self.out_file = out_file
        #err file is defined only when doing eff test
        self.err_file = err_file
        self.process = None
    
    def run(self, timeout):
#        def target():
        input = open(self.in_file)
        output = open(self.out_file, 'w')
        if self.err_file:
            err = open(self.err_file, 'w')
            self.process = \
                subprocess.Popen(self.command, stdin=input, stdout=output, stderr=err)
            #subprocess.Popen("time "+self.command, stdin=input, stdout=output, stderr=err, shell=True)
        else:
            self.process = \
                    subprocess.Popen(self.command, stdin=input, stdout=output)
        try:
            self.process.wait(timeout=timeout)
            output.flush()
            if self.err_file:
                err.flush()
        except (subprocess.TimeoutExpired) as err:
            self.process.terminate()
            return -15
        except (KeyboardInterrupt, Exception) as err:
            self.process.terminate()

#        thread = threading.Thread(target=target)
#        thread.start()
#        thread.join(timeout)
#        if thread.is_alive():
#            self.process.terminate()
#            thread.join()

        return self.process.returncode

#starting everything...
main()
