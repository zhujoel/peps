import pandas as pd
import matplotlib.pyplot as plt
import sys


if len(sys.argv) > 1:
    df = pd.read_csv(sys.argv[1], index_col=0)
    for i in range(len(sys.argv)-2):
        df.loc[:, sys.argv[i+2]].plot(figsize=(16,8), title="plot")
    plt.legend()
    plt.show()
else:
    print("command format: python plot_graph.py <column_name>")