$SFMLDownloadUrl = "https://www.sfml-dev.org/files/SFML-3.0.0-windows-vc17-64-bit.zip"
Invoke-WebRequest $SFMLDownloadUrl -OutFile "$($PSScriptRoot)\SFML.zip"
Expand-Archive -Path "$($PSScriptRoot)\SFML.zip" -DestinationPath $PSScriptRoot
return 0