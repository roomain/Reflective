import re
import sys
from pathlib import Path
import os
import datetime

commentary_pattern = r'(?:\s|//[^\n]*|/\*.*?\*/)*'
reflective_pattern = (
    r'(?:REFLECT_CLASS\s*\n)' + commentary_pattern +
    r'(?:class|struct)\s+(\w+)' + commentary_pattern +
    r'(?:\s*:\s*(?:(?:public|private|protected)\s+)?(\w+))?' + commentary_pattern +
    r'\{([^}]*)\}'
)

member_pattern = commentary_pattern + r'\s*([^;]+?)\s+([a-zA-Z_]\w*)\s*(?:=\s*[^;]*)?\s*;'

class ReflectiveClass:
    def __init__(self, name, base_class=None):
        self.name = name
        self.base_class = base_class
        self.members = []

    def add_member(self, type_str, name):
        self.members.append((type_str.strip(), name.strip()))


class ReflectiveParser:
    def __init__(self, filepath : str):
        self.filepath = filepath
        self.classes : dict[str, ReflectiveClass] = {}

    def parse(self, code: str):
        for match in re.finditer(reflective_pattern, code, re.DOTALL):
            class_name = match.group(1)
            base_class = match.group(2)
            body = match.group(3)

            reflective_class = ReflectiveClass(class_name, base_class)
            for member_match in re.finditer(member_pattern, body):
                type_str = member_match.group(1) 
                member_name =  member_match.group(2)
                reflective_class.add_member(type_str, member_name)
            self.classes[class_name] = reflective_class

    def printStats(self):
        count = len(self.classes.items())
        if count > 0:
            print("Reflective count {}".format(count))
            for class_name, reflective_class in self.classes.items():
                print("- {} ({} members)".format(class_name, len(reflective_class.members)))
                for type, name in reflective_class.members:
                    print('   {} {}'.format(type, name))

    def generateHeader(self):
        if len(self.classes) > 0:
            filename = os.path.basename(self.filepath)
            filename = filename[0:len(filename) - 1] + "generated.h"
            generatedFile = self.filepath.parent.absolute().__str__() + "\\\\" + filename
            file = open(generatedFile, 'w')
            dateNow = datetime.date.today()
            file.write("#pragma once\n")
            file.write("/***********************************************\n")
            file.write("* @headerfile {}\n".format(filename))
            file.write("* @date {}/{}/{}\n".format(dateNow.day, dateNow.month, dateNow.year))
            file.write("* @author reflective.py\n")
            file.write("************************************************/\n")
            file.write("#include <utility>\n")
            file.write("#include <tuple>\n")
            file.write("#include <string_view>\n")
            file.write("#include \"Reflective.h\"\n")
            for class_name, reflective_class in self.classes.items():
                file.write("\n\n")
                file.write("#define REFLECT_DEF_{} \\\n".format(class_name))
                #file.write("static inline bool s_deserialized = false; \\\n")
                #file.write("static {} s_reference; \\\n".format(class_name))
                tupleContent = ""
                memberCount = len(reflective_class.members)
                for index in range(memberCount):
                    member = reflective_class.members[index]

                    if index == (memberCount - 1): #last member
                        tupleContent += "std::pair<std::string_view, {} {}::*> \\\n".format(member[0], class_name)
                    else:
                        tupleContent += "\t\t\tstd::pair<std::string_view, {} {}::*>, \\\n".format(member[0], class_name)

                file.write("static std::tuple<{}> s_reflectiveCtx; \\\n \\\n".format(tupleContent))


                #################################################################################################################
                file.write("inline {}() \\\n".format(class_name))
                file.write("{ \\\n")
                #file.write("\tif(!{}::s_deserialized) \\\n".format(class_name))
                #file.write("\t{ \\\n")
                #file.write("\t\t{}::s_deserialized = Reflective::instance().deserialize(\"{}\",{}::s_reference); \\\n".format(class_name, class_name, class_name))
                #file.write("\t} \\\n")
                #file.write("\t*this = s_reference; \\\n")       
                file.write("\tReflective::instance().deserialize(\"{}\",*this); \\\n".format(class_name))
                file.write("}\n")
                #################################################################################################################
                file.write("\n\n")
                file.write("#define REFLECT_STATIC_IMPL_{} \\\n".format(class_name))
                #file.write("{} {}::s_reference; \\\n".format(class_name, class_name))
                
                tupleMakeContent = ""
                for index in range(memberCount):
                    member = reflective_class.members[index]
                    if index == (memberCount - 1):
                        tupleMakeContent += "std::make_pair(\"{}\", &{}::{})".format(member[1], class_name, member[1])
                    else:
                        tupleMakeContent += "std::make_pair(\"{}\", &{}::{}), \\\n\t".format(member[1], class_name, member[1])

                file.write("std::tuple<{}> {}::s_reflectiveCtx = std::make_tuple({});\n".format(tupleContent, class_name, tupleMakeContent))





print("    ____  ______________    __________________   _____________   ____________  ___  __________  ____ ")
print("   / __ \\/ ____/ ____/ /   / ____/ ____/_  __/  / ____/ ____/ | / / ____/ __ \\/   |/_  __/ __ \\/ __ \\")
print("  / /_/ / __/ / /_  / /   / __/ / /     / /    / / __/ __/ /  |/ / __/ / /_/ / /| | / / / / / / /_/ /")
print(" / _, _/ /___/ __/ / /___/ /___/ /___  / /    / /_/ / /___/ /|  / /___/ _, _/ ___ |/ / / /_/ / _, _/ ")
print("/_/ |_/_____/_/   /_____/_____/\\____/ /_/     \\____/_____/_/ |_/_____/_/ |_/_/  |_/_/  \\____/_/ |_|  ")
print("Parse header files to generate deserialize functions")
print("Search path: {}".format(sys.argv))
quiet =  sys.argv.__contains__("-q")
parseArgIndex = 1
if len(sys.argv) > 1:
    print("Parse headers from {}".format(sys.argv[parseArgIndex]))
    headers = []
    #process input headers
    for filePath in Path(sys.argv[1]).rglob('*.h'):
        file = open(filePath, 'r')
        code = file.read()
        file.close()
        parser = ReflectiveParser(filePath)
        parser.parse(code)
        if not quiet:
            parser.printStats()
        headers.append(parser)

    #generate output headers
    for generator in headers:
        generator.generateHeader()
else:
    print("Missing path argument")