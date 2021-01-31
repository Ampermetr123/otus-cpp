import os
project_name = "async"

text = '''{
    "package": {
        "name": "%s",
        "repo": "otus-cpp",
        "subject": "ampermetr123",
        "desc": "Homework 8 of OTUS-CPP course",
        "website_url": "https://ampermetr123.github.io/otus-cpp/",
        "issue_tracker_url": "https://github.com/Ampermetr123/otus-cpp/issues",
        "vcs_url": "https://github.com/Ampermetr123/otus-cpp.git",
        "licenses": ["GPL-2.0"]
    },

    "version": {
        "name": "0.0.%s",
        "desc": "This is a version"
    },

    "files":
        [{  
            "includePattern": "%s/build/(.*\.deb)", 
            "uploadPattern": "pool/main/%s/%s/$1",
            "matrixParams": { "deb_distribution": "trusty", "deb_component": "main", "deb_architecture": "amd64"}
        }],

    "publish": true

}''' % (project_name, os.getenv('TRAVIS_BUILD_NUMBER',0), project_name, project_name[0], project_name)

with open("./deploy.txt","w") as f:
    f.write(text)


