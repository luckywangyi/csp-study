import subprocess
import pathlib

BASE = pathlib.Path(r"C:\Users\wps\csp-study\problems")


def check(exe: pathlib.Path, td: pathlib.Path, name: str) -> None:
    for i in range(1, 11):
        inp = td / f"{i}.in"
        exp = (td / f"{i}.out").read_text(encoding="utf-8").strip()
        p = subprocess.run([str(exe)], input=inp.read_bytes(), capture_output=True)
        got = p.stdout.decode().strip()
        if got != exp:
            print(name, "FAIL", i, "got", repr(got), "exp", repr(exp))
    print(name, "checked")


if __name__ == "__main__":
    root = pathlib.Path(r"C:\Users\wps\csp-study")
    check(root / "_p4.exe", BASE / "201604" / "t1-折点计数" / "testcases", "P4")
    check(root / "_p5.exe", BASE / "201612" / "t1-中间数" / "testcases", "P5")
    check(root / "_p6.exe", BASE / "202104" / "t2-邻域均值" / "testcases", "P6")
