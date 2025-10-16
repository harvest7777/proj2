# Target: executable
app: main.c
	gcc -o dining main.c

# Clean target
clean:
	rm -f dining 

