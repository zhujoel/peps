import pandas as pd
import matplotlib.pyplot as plt
import sys

df = pd.read_csv('file', index_col=0)

if len(sys.argv) > 1:
    for i in range(len(sys.argv)-1):
        df.loc[:, sys.argv[i+1]].plot(figsize=(16,8), title="plot")
    plt.legend()
    plt.show()
else:
    print("command format: python plot_graph.py <column_name>")