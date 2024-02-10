MAKEFLAGS += --silent

GO := "C:\Program Files (x86)\Go\bin\go.exe"
RM := del

all: run

aes: aes.go
	$(GO) build aes.go sbox.go

run: aes
	aes.exe
	make clean

clean:
	$(RM) *.exe
