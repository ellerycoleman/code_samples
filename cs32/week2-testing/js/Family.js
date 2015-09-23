
var Family = function(){
	// Initialize
	this.family = JSON.parse(sessionStorage.getItem('family') || [];
    this.listenAddPerson();

	this.updateList();
};


Family.prototype.listenAddPerson = function()
{
	var that= this;
	$('#addPersonForm').on('submit', function(event){
		                                that.addPerson($('#person').val());
									    $('#person').val('');
									    return false;
	                                  });
};

Family.prototype.addPerson = function(person)
{   this.family.push(person);
	this.updateList();
};

Family.prototype.removeSong = function(index)
{
    this.playlist.splice(index,1);
	this.updatePlaylist();
};




