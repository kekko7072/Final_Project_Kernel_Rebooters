# Building the Report

The report is compiled with `pdflatex`. If LaTeX is not available, install a minimal set of packages:

```bash
sudo apt-get update
sudo apt-get install -y texlive-latex-base texlive-latex-recommended texlive-latex-extra texlive-fonts-recommended
```

Then build from this folder:

```bash
make
```

The Makefile runs `pdflatex` twice and cleans auxiliary files, leaving the final `report_Kernel_Rebooters_Vision_Division.pdf`.
