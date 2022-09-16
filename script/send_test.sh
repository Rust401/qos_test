make arm64

echo "bin for rk3568 generated!"
cp qos_test /mnt/c/ext/OH-full/qos_tester/qos_test
echo "bin copy to windows!"

sshpass -p 'Lvxy*9910' ssh -t ruty@7.249.248.5 "C:\ext\OH-full\qos_tester\push_test.bat"
