#!/usr/bin/python

import getopt, sys
import re
import os

##########################################################
class LongOptString:
   def __init__(self, optString, example):
      self.optString = optString
      self.example = example

   def toAssignable(self):
      return self.optString + '='

   def toLongOptString(self):
      return '--' + self.optString

   def toLongAssignable(self):
      return '--' + self.optString + '='

   def toUsage(self):
      return self.toLongAssignable() + self.example

##########################################################
def getAssignableOptStrings(longOpts):
   return [i.toAssignable() for i in longOpts]

##########################################################
def getUsageString(longOpts):
   return " ".join([i.toUsage() for i in longOpts])

##########################################################
def usage(longOpts):
   print "Usage:", sys.argv[0], getUsageString(longOpts), "-o target testcase1 testcase2 ..."

##########################################################
def getOpt(longOpts):
   try:
      return getopt.getopt(sys.argv[1:], 'o:', getAssignableOptStrings(longOpts))
   except getopt.GetoptError, err:
      print >> sys.stderr, str(err)
      usage(longOpts)
      sys.exit(2)

allFixtures = []
fixture_re = re.compile( r'^(?P<fixtureDef>\s*(class|struct)\s*(?P<fixtureName>[A-Za-z_]\w*)\s*:\s*public\s+((testcpp|TESTCPP_NS)\s*::)?\s*TestFixture\w*)' )
case_re = re.compile( r'^(?P<caseDef>\s*void\s+(?P<caseName>test\w*)\(\s*(void){0,1}\s*\))' )
if0_re = re.compile( r'^\s*#\s*if\s+0\s*(//.*)?$' )
if0_re2 = re.compile( r'^\s*#\s*if\s+0\s*(/\*.*\*/\s*)?$' )
endif_re = re.compile( r'\s*#\s*endif\s*(//.*)?$' )
endif_re2 = re.compile( r'\s*#\s*endif\s*(/\*.*\*/\s*)?$' )
ifdef_re = re.compile( r'\s*#\s*ifdef\s+\w+' )
ifndef_re = re.compile( r'\s*#\s*ifndef\s+\w+' )
if_re = re.compile( r'\s*#\s*if\s+.*$' )

class FixtureParser:
   def __init__(self, fixture, fixtureDef):
      self.fixture = fixture
      self.fixtureDef = fixtureDef
      self.lineNo = fixture['line']
      self.enterFixture = None
      self.leftFixture = None
      self.inFixture = None
      self.unmatchedLeftBraces = 0
      self.finishFixture = None

      self.inComment = None
      self.mightBeComment = None
      self.mightEndComment = None
      self.defLine = True

   def getLineNo(self):
      return self.lineNo

   def parse(self, line):
      self.parseLine(self.defLine, line)
      self.lineNo += 1
      self.defLine = None

      return self.finishFixture

   def tryToLeftComment(self, char):
      if char == '*' and self.inComment:
         self.mightEndComment = True
      elif char == '/' and self.mightEndComment:
         self.inComment = None
         self.mightBeComment = None
         self.mightEndComment = None
      else:
         self.mightBeComment = None
         self.mightEndComment = None

      return True

   def checkComment(self, char):
      if char == '/' and self.mightBeComment: ## Single-Line Comment
         self.mightBeComment = None
         self.mightBeComment = None
         return 1 
      elif char == '/' and not self.mightBeComment: ## Might Be Comment 
         self.mightBeComment = True
         self.mightBeComment = None
         return 2
      elif char == '*' and self.mightBeComment: ## Multi-Line Comment
         self.mightBeComment = None
         self.inComment = True
         return 2

      self.mightBeComment = None
      self.mightBeComment = None

      return 0
      
   def onlyCommentsIsAllowed(self, char):
      result = self.checkComment(char)
      if result == 1:
         return None
      elif result == 2:
         return True

      print >> sys.stderr, "Grammar error in line", self.lineNo
      sys.exit(1)
    
   def tryToEnterFixture(self, char):
      if self.inComment:
         return self.tryToLeftComment(char)

      if char == '{' and not self.mightBeComment:
         self.unmatchedLeftBraces = 1
         self.enterFixture = True
         self.inFixture = True
         return True
      
      return self.onlyCommentsIsAllowed(char)

      
   def tryToFinishFixture(self, char):
      if self.inComment:
         return self.tryToLeftComment(char)

      if char == ';' and not self.mightBeComment:
         self.finishFixture = True
         return None

      return self.onlyCommentsIsAllowed(char)

   def handleInFixture(self, char):
      if self.inComment:
         return self.tryToLeftComment(char)

      if char == '{':
         self.unmatchedLeftBraces += 1
         return True

      if char == '}':
         self.unmatchedLeftBraces -= 1
         if self.unmatchedLeftBraces == 0:
            self.leftFixture = True
            self.inFixture = False
         return True
            
      result = self.checkComment(char)
      if result == 1:
         return None

      return True

   def handleChar(self, char):
      if char.isspace():
         return True

      if not self.enterFixture:
         return self.tryToEnterFixture(char)
      elif self.leftFixture:
         return self.tryToFinishFixture(char)

      return self.handleInFixture(char)

   def validateAndAddTestCase(self, testcase):
      for case in self.fixture['tests']:
         if case['name'] == testcase['name']:
            fatalError(self.fixture['file'], self.lineNo, "duplicated testcases with name " + testcase['name'])

      self.fixture['tests'].append(testcase)

   def parseLine(self, defLine, line):
      chars = line
      if defLine:
         chars = line[len(self.fixtureDef):]

      if self.enterFixture and self.inFixture and not self.inComment and self.unmatchedLeftBraces == 1: # Might Be a test case definition
         m = case_re.match(line)
         if m:
            testcase = {
               'name' : m.group('caseName'),
               'line' : self.lineNo
            }
            self.validateAndAddTestCase(testcase)
            chars = line[len(m.group('caseDef')):]

      for i in range(0, len(chars)):
         if self.handleChar(chars[i]) == None:
            return

##########################################################
class FixtureFileParser:
   def __init__(self, lines, lineNo, fixtureFile):
      self.lines = lines
      self.lineNo = lineNo
      self.fixtureFile = fixtureFile
      self.inIf0 = None
      self.inFixture = None
      self.fixtureParser = None
      self.fixture = None
      self.unmatchedIf0s = 0
      self.ifMacroStack = []

   def popupMacroStack(self):
      if len(self.ifMacroStack) == 0:
         fatalError(self.fixtureFile, self.lineNo, "unexpected #endif")
      if self.ifMacroStack[-1] == 1:
         self.unmatchedIf0s -= 1

      del self.ifMacroStack[-1:]

   def tryToFindFixture(self, line):
      m = fixture_re.match(line)
      if not m:
         return

      self.createFixture(line, m.group("fixtureName"), m.group('fixtureDef'))

   def createFixture(self, line, fixtureName, fixtureDef):
      self.fixture = { 'name' : fixtureName,
                       'file' : self.fixtureFile,
                       'line' : self.lineNo,
                       'tests': []
                     }

      self.fixtureParser = FixtureParser(self.fixture, fixtureDef)
      self.fixtureParser.parse(line)
      self.inFixture = True
      
   def validateAndAddFixture(self, newFixture):
      for fixture in allFixtures:
         if fixture['name'] == newFixture['name']:
            fatalError(self.fixtureFile, self.lineNo, "duplicated fixture definition with name " + fixture['name'])

      allFixtures.append(newFixture)
      
   def tryToProcessFixture(self, line):
      if self.fixtureParser.parse(line): # fixture def ended
         self.inFixture = None
         self.validateAndAddFixture(self.fixture)

   def pushIf0ToStack(self):
      self.ifMacroStack.append(1)
      self.unmatchedIf0s += 1

   def handleLine(self, line):
      m = if0_re.match(line) or if0_re2.match(line)
      if m:
         self.pushIf0ToStack()
         return

      m = if_re.match(line) or ifdef_re.match(line) or ifndef_re.match(line)
      if m:
         self.ifMacroStack.append(0)
         return

      m = endif_re.match(line) or endif_re.match(line)
      if m:
         self.popupMacroStack()
         return 

      if self.unmatchedIf0s > 0:
         return 

      if self.inFixture:
         return self.tryToProcessFixture(line)
      
      return self.tryToFindFixture(line)

   def parse(self):
      for line in self.lines:
         self.handleLine(line)
         self.lineNo += 1

      if len(self.ifMacroStack) > 0:
         fatalError(self.fixtureFile, self.lineNo, "Unmatched #if/#ifdef and #endif")

      if self.inFixture:
         fatalError(self.fixtureFile, self.lineNo, \
                  "the definition of fiture " + self.fixture['name'] + " is not closed.")

def fatalError(file, line, error):
   print >> sys.stderr, file+":"+str(line)+":fatal error:", error
   sys.exit(1)

##########################################################
def trimCRLF(line):
   if line.endswith('\r\n'):
      return line[:-2]
   elif line.endswith('\n'):
      return line[:-1]
   return line

##########################################################
def processTestFixtureFile(fixtureFile):
   FILE = file(fixtureFile)

   lines = []
   for line in FILE:
      lines.append(trimCRLF(line))

   parser = FixtureFileParser(lines, 1, fixtureFile)
   parser.parse()

   FILE.close()

##########################################################
def checkFixtureExistence(fixtureFiles):
   for fixtureFile in fixtureFiles:
      if not os.path.exists(fixtureFile):
         print >> sys.stderr, fixtureFile, "does not exist"
         sys.exit(1)

##########################################################
depHeaders = [
   "internal/TestCase.h",
   "internal/TestFixtureDesc.h",
   "internal/TestSuiteDesc.h"
]

##########################################################
def generateHeaders(FILE, fixtureFiles):
   includeDepHeaders = ["#include <testcpp/" + header + ">\n" for header in depHeaders]
   includeFixtureFiles = ["#include \"" + header + "\"\n" for header in fixtureFiles]
   FILE.writelines(includeDepHeaders)
   FILE.writelines(includeFixtureFiles)
   
##########################################################
def getFixtureDefName(fixture):
   return "dont_use_this_fixture_prefix_" + fixture['name']

##########################################################
def getTestCaseClassName(fixture, testcase):
   return "TestCppGeneratedTestCase_" + fixture['name'] + "_" + testcase['name']

##########################################################
def getTestCaseDefName(fixture, testcase):
   return "testCppGeneratedTestCase_" + fixture['name'] + "_" + testcase['name']

##########################################################
def getFixtureCaseArrayName(fixture):
   return "testCppGeneratedTestCaseArray_" + fixture['name']

##########################################################
def getFixtureDescName(fixture):
   return "testCppGeneratedTestFixtureDesc_" + fixture['name']

##########################################################
def getFixtureDescArrayName(fixture):
   return "testCppGeneratedTestFixtureDescArray_" + fixture['name']

##########################################################
def getSuiteDescName(fixture):
   return "testCppGeneratedTestSuiteDesc_" + fixture['name']

##########################################################
def generateTestCase(FILE, fixture, testcase):
   testcaseDef = "static struct " + getTestCaseClassName(fixture, testcase) + " : public testcpp::TestCase {\n" + \
                 "   " + getTestCaseClassName(fixture, testcase) + "()\n" + \
                 "      : testcpp::TestCase(\"" + testcase['name']+"\",\"" + \
                 fixture['name'] + "\",\"" + fixture['file'] + "\","+ str(testcase['line']) +") \n" + \
                 "      {}\n" + \
                 "   void run() {\n" + \
                 "      " + getFixtureDefName(fixture) + "." + testcase['name'] + "();\n" + \
                 "   }\n" + \
                 "}" + getTestCaseDefName(fixture, testcase) + "; \n"

   FILE.writelines(testcaseDef)

##########################################################
def generateTestCaseArray(FILE, fixture):
   testcaseArrayDef = "static testcpp::TestCase* " + getFixtureCaseArrayName(fixture) + "[] = {\n"
   for testcase in fixture['tests']:
      testcaseArrayDef += "   &" + getTestCaseDefName(fixture, testcase) + ",\n"
   testcaseArrayDef += "};\n"
   FILE.writelines(testcaseArrayDef)
   
##########################################################
def generateFixtureDesc(FILE, fixture):
   fixtureDescDef = "static testcpp::TestFixtureDesc " + getFixtureDescName(fixture) + "\n" + \
                    "   (\"" + fixture['name'] + "\",\"" + fixture['file'] + "\",\n" + \
                    "    &" + getFixtureDefName(fixture) + "," + getFixtureCaseArrayName(fixture) + "," + str(len(fixture['tests'])) + ");\n"

   FILE.writelines(fixtureDescDef)

##########################################################
def generateTestCases(FILE, fixture):
   for testcase in fixture['tests']:
      generateTestCase(FILE, fixture, testcase)
      
   generateTestCaseArray(FILE, fixture)
   generateFixtureDesc(FILE, fixture)

##########################################################
def generateFixture(FILE, fixture):
   fixtureDef = "static " + fixture['name'] + " " + getFixtureDefName(fixture) + ";\n";
   FILE.writelines(fixtureDef)
   generateTestCases(FILE, fixture)

##########################################################
def generateFixtureArray(FILE, fixtures):
   fixtureArrayDef = "static testcpp::TestFixtureDesc* " + getFixtureDescArrayName(fixtures[0]) + "[] = {\n" 
   for fixture in fixtures:
      fixtureArrayDef += "   &" + getFixtureDescName(fixture) + ",\n"
   fixtureArrayDef += "};\n"
   FILE.writelines(fixtureArrayDef)

def generateSuiteDef(FILE, suiteName, fixtures):
   suiteDescDef = "static testcpp::TestSuiteDesc " + getSuiteDescName(fixtures[0]) + "(\n" + \
                  "   \"" + suiteName + "\"," + getFixtureDescArrayName(fixtures[0]) + "," + str(len(fixtures)) + ");\n"

   FILE.writelines(suiteDescDef)

def generateSuiteDescGetter(FILE, fixtures):
   suiteDescGetter = "extern \"C\" testcpp::TestSuiteDesc* ___testcpp_test_suite_desc_getter() { \n" + \
                     "   return &" + getSuiteDescName(fixtures[0]) + ";\n" + \
                     "} \n"
   FILE.writelines(suiteDescGetter)
##########################################################
def generateSuite(FILE, suiteName, fixtures):
   generateFixtureArray(FILE, fixtures)
   generateSuiteDef(FILE, suiteName, fixtures)
   generateSuiteDescGetter(FILE, fixtures)

##########################################################
def generateFixtures(FILE, suiteName, fixtures):
   for fixture in fixtures:
      generateFixture(FILE, fixture)

   generateSuite(FILE, suiteName, fixtures)

##########################################################
def generateTarget(target, fixtureFiles, allFixtures):
   try:
      FILE = open(target, "w")
   except IOError:
      print >> sys.stderr, "open", target, "failed"
      sys.exit(1)

   generateHeaders(FILE, fixtureFiles)

   if len(allFixtures) > 0:
      generateFixtures(FILE, os.path.basename(target).split('.')[0], allFixtures)

   FILE.close()

##########################################################
def process(target, fixtureFiles):
   checkFixtureExistence(fixtureFiles)


   for fixtureFile in fixtureFiles:
      processTestFixtureFile(fixtureFile)

   global allFixtures
   generateTarget(target, fixtureFiles, allFixtures)


##########################################################
longOpts = []

##########################################################
def main():
   optlist, fixtures = getOpt(longOpts)

   target = None

   for o, a in optlist:
      if o == "-o":
         target = a

   if target == None:
      usage(longOpts)
      sys.exit(1)

   process(target, fixtures)

##########################################################
if __name__ == "__main__":
   main()

