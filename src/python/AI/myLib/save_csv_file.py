#!/usr/bin/python3

import pandas as pd

file = ""
save_path = "data/"

parcalar = file.split("/")
file_name = parcalar[-1].replace("-", "_") 

data = pd.read_csv(file)
data.to_csv(save_path + file_name)


