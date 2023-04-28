/* */

function prikaz(id){
	var p = document.getElementById(id);
	if (p.style.display === "none") {
		p.style.display = "block";
	} else {
		p.style.display = "none";
	}
}
	
function sakrij(ids){
	var len=ids.length, i=0;
	for (i=0; i < len; i++) {
		document.getElementById(ids[i]).style.display="none";
	}
}
function jednaki(v1,v2){
	return v1===v2;
}

function prikazisamo(ids,prikazi){
	var li=ids.length, lp=prikazi.length;
	var postoji=false, i=0, j=0;
	for (i=0; i < li; i++){
		postoji=false;
		for (j=0; j<lp; j++){
			if (ids[i]===prikazi[j]){
				document.getElementById(prikazi[j]).style.display="block";
				postoji=true;
			}
			if (!postoji) {
			document.getElementById(ids[i]).style.display="none";
			}
		}
		
		
	}
}