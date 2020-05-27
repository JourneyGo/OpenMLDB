self_dir=$(dirname $0)
cd ${self_dir}
python3 generate_table_file.py
IP=127.0.0.1
ZK_CLUSTER=$IP:6181
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='drop auto'
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='drop ck'
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='drop date'
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='drop rt_ck'
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='drop test1'
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='create auto.txt'
sleep 3
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='create ck.txt'
sleep 3
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='create date.txt'
sleep 3
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='create rt_ck.txt'
sleep 3
${WORKDIR}/build/bin/rtidb --zk_cluster=${ZK_CLUSTER} --zk_root_path=/onebox --role=ns_client --interactive=false --cmd='create rt.txt'
cd -
