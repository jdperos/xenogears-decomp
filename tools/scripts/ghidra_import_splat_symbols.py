# Ghidra script which parses splat symbol files

# Possible improvements:
# - Import struct definitions
# - Look up function signature if there is one, and set it

from ghidra.program.model.symbol import SourceType
from ghidra.app.cmd.function import CreateFunctionCmd

def define_function(func_addr, name):
    func_addr = currentProgram.getAddressFactory().getAddress(str(hex(func_addr)))
    functionManager = currentProgram.getFunctionManager()
    func = functionManager.getFunctionAt(func_addr)
    if func is None:
        func_body = CreateFunctionCmd(func_addr)
        functionManager.createFunction(name, func_addr, func_body.getFunctionBody(currentProgram, func_addr), SourceType.USER_DEFINED)
    else:
        func.setName(name, SourceType.USER_DEFINED)

def define_symbol(addr, name):
    addr = currentProgram.getAddressFactory().getAddress(str(hex(addr)))
    currentProgram.getSymbolTable().createLabel(addr, name, SourceType.USER_DEFINED)

def parse_symbol_file(f):
    for line in file(f.absolutePath):
        # Normalize line
        line = line.strip()
        line = line.replace(" ", "")
        if line.startswith("//") or len(line) < 4:
            continue
        
        # Parse symbol name and address
        parts = line.split("=")
        name = parts[0]
        addr = int(parts[1].split(";")[0], 16)
        print(name, hex(addr))

        if name.startswith("g_"):
            define_symbol(addr, name)
        else:
            define_function(addr, name)

f = askFile("Choose a splat symbol file", "")
parse_symbol_file(f)