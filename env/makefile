all:env myshell mini

env:env.c
	gcc $^ -o $@

myshell:myminishell.c
	gcc $^ -o $@ -g
	
mini:minishell.c
	gcc $^ -o $@ -g
