* goto ~/seqtk_wrapper (where your tool files are located) and create a .shed.yml file (containg the tool description in the toolshed format):

''' planemo shed_init --name=seqtk_seq --owner=planemo --description=seqtk_seq --long_description="Tool that converts FASTQ to FASTA files using seqtk" --category="Fastq Manipulation" '''

* open the file ~/.planemo.yml and get the username and password
* goto the local test toolshed (from desktop). register a user with the username and password from 2
* login to the toolshed with this user --> goto "Manage categories" and create a new category "Fastq Manipulation"
* Now goto ~/seqtk_wrapper (where your tool files are located) push your tool to the local toolshed:

''' planemo shed_create --shed_target local '''
