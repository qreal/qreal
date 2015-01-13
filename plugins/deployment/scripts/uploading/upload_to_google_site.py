#!/usr/bin/python

usage = ("Usage: upload_to_google_site site_name username password <a list of tripples: url, title and description>\n"
	"Example: upload_to_google_site qrealrobots login pa$$word \"http://qreal.ru/downloadCounter.html?target=https://docs.google.com/file/d/0B-NmiVtuyhyiMEU0WjR0MEp1YkU\" \"TRIKStudio 3.0.0 Release Candidate 2\" \"Online installer for Windows\"\n\n"
	"Adds new links to the filecabinet of the given google site. The site is identified by the google identifier (for example, qrealrobots for robots.qreal.ru) which is passed as the first argument.\n"
	"The second and the third arguments are email (or username) and password of site admin`s google account correspondingly.\n"
	"Following information should contain 3N arguments where N is the number of uploaded links.\n"
	"Each tripple must contain first URL of the linked resource, second the title of the uploaded link and finally the description of the resource.\n"
	"See example for more clarification.\n"
)

import sys
import gdata.sample_util
import gdata.sites.client
import gdata.sites.data

if (len(sys.argv) < 7) or (len(sys.argv) % 3 != 1):
	exit(usage);

links_count = (len(sys.argv) - 4) / 3;
site = sys.argv[1]
username = sys.argv[2]
password = sys.argv[3]

print "Now the given links will be uploaded to", site, "file cabinet..."

SOURCE_APP_NAME = 'qreal-GoogleSitesUploader-v1'
client = gdata.sites.client.SitesClient(source=SOURCE_APP_NAME, site=site)

print "Authenticating..."
try:
	client.client_login(username, password, source=SOURCE_APP_NAME, service=client.auth_service);
except gdata.client.BadAuthentication:
	exit('Invalid user credentials given.')
except gdata.client.Error:
	exit('Login Error')
print "Authentication successfull!"

uri = '%s?kind=%s' % (client.MakeContentFeedUri(), 'filecabinet')
feed = client.GetContentFeed(uri=uri)
parent_entry = feed.entry[0]

for i in range(links_count):
	url = sys.argv[4 + 3*i];
	title = sys.argv[5 + 3*i];
	description = sys.argv[6 + 3*i];
	print "Uploading", description, "..."
	web_attachment = client.CreateWebAttachment(url, 'application/octet-stream', title, parent_entry, description=description)
