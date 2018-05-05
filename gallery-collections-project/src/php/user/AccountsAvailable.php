<?php
class AccountsAvailable {
	static $FILE_PATH = '';
	private $numAccountsAvailable;

  function __construct($config) {
  	AccountsAvailable::$FILE_PATH = $config['base_path'] . $config['user_db_path'] . 'accounts_available.txt';

    while(!($file = fopen(AccountsAvailable::$FILE_PATH, 'r'))) {}
    $this->numAccountsAvailable = (int) fread($file, filesize(AccountsAvailable::$FILE_PATH));
    fclose($file);
  }

  final function isAvailable() {
    return $this->numAccountsAvailable > 0;
  }

  final function consumeAccount() {
    while(!($file = fopen(AccountsAvailable::$FILE_PATH, 'r+'))) {}
    $this->numAccountsAvailable = (int) fread($file, filesize(AccountsAvailable::$FILE_PATH));
    if($this->isAvailable()) {
      rewind($file);//ftruncate($file, 0);
      fwrite($file, --$this->numAccountsAvailable);
    }
    fclose($file);
  }

  final function replaceAccount() {
    while(!($file = fopen(AccountsAvailable::$FILE_PATH, 'r+'))) {}
    $this->numAccountsAvailable = (int) fread($file, filesize(AccountsAvailable::$FILE_PATH));
    rewind($file);//ftruncate($file, 0);
    fwrite($file, ++$this->numAccountsAvailable);
    fclose($file);
  }
}
?>