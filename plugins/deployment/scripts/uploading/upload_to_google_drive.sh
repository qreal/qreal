 #!/bin/bash

# Improvement of https://gist.github.com/deanet/3427090
# Pass --help option for getting useful information

set -o nounset
set -o errexit

if [ $# -lt 2 ]; then
    echo "Usage: upload_to_google_drive.sh file folder_id email [password]"
    exit -1
fi

file="$1"
slug=`basename "$file"`
folder_id=$2
browser="Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:13.0) Gecko/20100101 Firefox/13.0.1"
username=$3
[ $# -eq 3 ] && read -s -p "Enter password: " password || password=$4
accountype="GOOGLE" #gooApps = HOSTED , gmail=GOOGLE
mime_type=`file -b --mime-type $file`

echo "Uploading $1 to Google Drive..."

/usr/bin/curl -v --data-urlencode Email=$username --data-urlencode Passwd=$password -d accountType=$accountype -d service=writely -d source=cURL "https://www.google.com/accounts/ClientLogin" > /tmp/login.txt

token=`cat /tmp/login.txt | grep Auth | cut -d \= -f 2`

uploadlink=`/usr/bin/curl -Sv -k --request POST -H "Content-Length: 0" -H "Authorization: GoogleLogin auth=${token}" -H "GData-Version: 3.0" -H "Content-Type: $mime_type" -H "Slug: $slug" "https://docs.google.com/feeds/upload/create-session/default/private/full/folder:$folder_id/contents?convert=false" -D /dev/stdout | grep "Location:" | sed s/"Location: "// | tr '\r' ' '`

/usr/bin/curl -Sv -k --request POST --data-binary "@$file" -H "Authorization: GoogleLogin auth=${token}" -H "GData-Version: 3.0" -H "Content-Type: $mime_type" -H "Slug: $slug" "$uploadlink" > /tmp/goolog.upload.txt

if [[ `cat /tmp/goolog.upload.txt` =~ .*https://docs.google.com/feeds/default/private/full/file%..(.*)/revisions.* ]]; then
	echo "Uploaded successfully, URL to download: https://docs.google.com/file/d/"${BASH_REMATCH[1]}
else
	echo "ERROR: File id could not be found in log file".
fi
