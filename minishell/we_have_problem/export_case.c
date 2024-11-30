yntax_error
--------------------------------------------
`env
`export
export $?
export TE+S=T=""	
export TES+T=123
export TES}T=123
export TE*ST=123	
export TES#T=123
export TES@T=123
export TES$?T=123
export +++++++=123
export TES^T=123	
export TEST+=100


not a valid identifier
--------------------------------------------
export TEST
export TEST=
export TEST=123
export ___TEST=123
export _TEST=100


false return
-------------------------------------------
export TES_T=123
export ________=123
export export
export echo
export pwd
export unset
