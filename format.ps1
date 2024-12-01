$files = (git ls-files --exclude-standard);
$extensions = ".cpp", ".h", ".hpp", ".c", ".cc", ".cxx", ".hxx", ".ixx"

foreach ($file in $files) {
    if ((Get-Item $file).Extension -in $extensions) {
        D:\MSVC\VC\Tools\Llvm\bin\clang-format.exe -i -style=file $file
    }
}