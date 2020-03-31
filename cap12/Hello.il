.assembly sample { }
.class public Hello extends [mscorlib]System.Object {
.method public static void  Main(string[] args) {
  .entrypoint
  .maxstack  8
  nop
  ldstr      "Olá pessoal, tudo fixe!"
  call       void [mscorlib]System.Console::WriteLine(string)
  nop
  ret
  }
}

