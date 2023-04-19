#SRC
ssh -i debian-img/bullseye.id_rsa -p 10021 -o "StrictHostKeyChecking no" root@localhost

#SNK
ssh -i debian-img/bullseye.id_rsa -p 10031 -o "StrictHostKeyChecking no" root@localhost
