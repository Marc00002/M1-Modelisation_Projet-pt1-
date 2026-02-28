import pandas as pd
import matplotlib.pyplot as plt

def plot_linear(csv_path, xcol, ycols, title, out_png):
    df = pd.read_csv(csv_path)

    plt.figure()
    for y in ycols:
        plt.plot(df[xcol], df[y], marker="o", label=y)
    plt.xlabel(xcol)
    plt.ylabel("temps moyen (s)")
    plt.title(title)
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png, dpi=200)
    plt.show()

if __name__ == "__main__":
    # (a) n fixé, b variable
    plot_linear(
        csv_path="bench_nfixed.csv",
        xcol="b",
        ycols=["greedy_sec", "lp_sec", "pd_sec"],   
        title="TP1 et TP2: n fixé, b variable",
        out_png="tp1_tp2_nfixed_linear.png",
    )


    # (b) b fixé, n variable
    plot_linear(
        csv_path="bench_bfixed.csv",
        xcol="n",
        ycols=["greedy_sec", "lp_sec", "pd_sec"], 
        title="TP1 et TP2: b fixé, n variable",
        out_png="tp1_tp2_bfixed_linear.png",
    )
