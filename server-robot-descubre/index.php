<?php
error_reporting(-1);
/**
 * Class that genereates and modify the jsons files
 */
class States
{
	
	public function __construct($post) {
		$this->executeCmdCommand($this->jsonToNumber($post));
	}
		
	private function executeCmdCommand($params){
		echo $params;
		//exec("cat /dev/ttyACM0");
		$fp =fopen("/dev/ttyACM0", "wr");
		if( !$fp) {
		        die("error");
		}
		fwrite($fp, $params);
		fclose($fp);
	}
		
		private function jsonToNumber($post){			
$number = '';
			//0 Off 1 On
			if ($post['query'] == 0){
				$number.='0';										
			}else if($post['query'] ==1){
				$number.='1';
			}
			
			//0 Windows 1 lights 2 doors 3 tour
			if ($post['what'] == 'table'){
				$number.='0';										
			}else if($post['what'] =='speakers'){
				$number.='1';
			}
			
			//0 lounge 1 kitchen 2 restroom 3 hall 4 studio 5 room 6 terrace
			if ($post['where'] == 'lounge'){
				$number.='0';										
			} elseif($post['where'] =='kitchen'){
				$number.='1';
			}elseif($post['where'] =='restroom'){
				$number.='2';
			}elseif($post['where'] =='hall'){//pasillo
				$number.='3';
			}elseif($post['where'] =='studio'){
				$number.='4';
			}elseif($post['where'] =='room'){
				$number.='5';
			}elseif($post['where'] =='terrace'){
				$number.='6';
			}elseif($post['where'] =='exteriors'){
				$number.='7';
			}

			$conversion = [
					'120' => '6',
					'020' => '7',
					'121' => '8',
					'021' => '9',
					'125' => '10',
					'025' => '11',
					'104' => '12',
					'004' => '13',
					'105' => '14',
					'005' => '15',
					'103' => '16',
					'003' => '17',
					'115' => '18',
					'015' => '19',
					'110' => '20',
					'010' => '21',
					'111' => '22',
					'011' => '23',
					'114' => '24',
					'014' => '25'
			];
			return $conversion[$number];
   	}
}

if($_POST['what'] || $_POST['where'] || $_POST['query'])
{
	$what=$_POST['what'];
	$where=$_POST['where'];
	$query=$_POST['query'];
	
	
	

	
	
}else{
	echo "nothing";
}
