Import("env")
import os, string, random

valiables = ["APP_EUI", "DEV_EUI", "APP_KEY"]

def readDotEnv():
    if not os.path.exists(".env"):
        return
    with open(".env", "rt") as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith("#"):
                key, value = line.split("=", 1)
                if value.startswith('"') and value.endswith('"'):
                    value = value[1:-1]
                os.environ[key] = value

defines = list()
readDotEnv()

for key in valiables:
    value = os.getenv(key, None)
    if value is None:
        continue
    defines.append((key, value))

env.Append(CPPDEFINES=defines)
