rem Script that generates new certificate authority with self-signed certificate and new code signing certificate for installer signing.
rem Just run this file with Windows SDK bin folder in PATH, then use CyberTechLabsSPC.pfx with sign.bat to sign the installer.
makecert -r -pe -n "CN=CyberTech Labs Ltd. CA" -ss CA -sr CurrentUser -a sha256 -cy authority -sky signature -sv CyberTechLabsCA.pvk CyberTechLabsCA.cer
makecert -pe -n "CN=CyberTech Labs Ltd." -a sha256 -cy end -sky signature -ic CyberTechLabsCA.cer -iv CyberTechLabsCA.pvk -sv CyberTechLabsSPC.pvk CyberTechLabsSPC.cer
pvk2pfx -pvk CyberTechLabsSPC.pvk -spc CyberTechLabsSPC.cer -pfx CyberTechLabsSPC.pfx
