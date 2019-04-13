# Install ansible if your computer doesn't have it yet

    sudo apt install ansible

# Run playbook for

    ansible-playbook -i localhost, -c local playbook.yml -b [-K]

-b : run commands with sudo (user -s in previous versions)
-K : ask sudo password
