PGP keys
========

This folder contains the public keys of developers and active contributors.

The keys are mainly used to sign git commits or the build results of gitian
builds.

You can import the keys into gpg as follows. Also, make sure to fetch the
latest version from the key server to see if any key was revoked in the
meantime.

```sh
gpg --import ./*.pgp
gpg --refresh-keys
```

# How to create keys

## GPG

1. Generate GPG Key

```
# generate key
gpg --gen-key

```

2. At the prompt, specify the kind of key you want, or press Enter to accept the default RSA and RSA.

3. Enter the desired key size. We recommend the maximum key size of 4096.

4. Enter the length of time the key should be valid. Press Enter to specify the default selection, indicating that the key doesn't expire.

5. Verify that your selections are correct.

6. Enter your user ID information. (**Note**: ___When asked to enter your email address, ensure that you enter the verified email address for your GitHub account___.)

## SSH

### Create new key
1. Generate a key, RSA 4096 bits fits to most users, 2048 is not secure anymore, **please use any value over 2048**.
`ssh-keygen -t rsa -b 4096 -C "your_email@example.com"`
(___if you want to be able to use this key on github, specify your github email address___)

2. start ssh in background `eval "$(ssh-agent -s)"`

3. add your newly created key `ssh-add ~/.ssh/id_rsa`


___You can copy and paste this script in your terminal___

```
# 1. generate rsa key with 4096 bits
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"

# 2. Start the ssh-agent in the background.
eval "$(ssh-agent -s)"

# 3. add your new new key
ssh-add ~/.ssh/id_rsa
```

### Add existing Key

1. start ssh in background `eval "$(ssh-agent -s)"`

2. add your newly created key `ssh-add /path/to/your/public_key_file"`

## Public Keys included in  gitian folder

- [Cevap](https://github.com/cevap/) - pub   512E/[A8D009CB](https://pgp.mit.edu/pks/lookup?op=get&search=0x5E07DE13A8D009CB) 2017-06-20 [🗺️Ionomy©️ 🐼 CE 🐼 RandomDevs 👩‍🔬👨‍🔬 (💂 Ionomy©️ CE Edition 💂) <ionomy@i2pmail.org>](https://pgp.mit.edu/pks/lookup?op=vindex&search=0x5E07DE13A8D009CB)
- [Ionomy CE Devs](https://github.com/ionmy/) pub  3072R/[BA6683BA](https://pgp.mit.edu/pks/lookup?op=get&search=0x49464B32BA6683BA) 2017-03-13 [developer (dev@i2pmail.org) <dev@i2pmail.org>](https://pgp.mit.edu/pks/lookup?op=vindex&search=0x49464B32BA6683BA)

```
-------------------------------
pub   rsa3072 2017-03-13 [SC]
      1F353A7100D09A436F7DF7C549464B32BA6683BA
uid           [ unknown] developer (dev@i2pmail.org) <dev@i2pmail.org>
sub   rsa3072 2017-03-13 [E]
-------------------------------
pub   brainpoolP512r1 2017-06-20 [SCA] [expires: 2019-06-20]
      3BECF4903CDDDBE22476F3055E07DE13A8D009CB
uid           [ unknown] 🗺️Ionomy©️ 🐼 CE 🐼 RandomDevs 👩‍🔬👨‍🔬 (💂 Ionomy©️ CE Edition 💂) <ionomy@i2pmail.org>
sub   brainpoolP512r1 2017-06-20 [E] [expires: 2019-06-20]
-------------------------------
```
