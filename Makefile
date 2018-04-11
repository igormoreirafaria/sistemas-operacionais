target: funcShellso.c shellso.c
	gcc funcShellso.c shellso.c -o shellso

clean: 
	rm -rf shellso

run: shellso
	./shellso