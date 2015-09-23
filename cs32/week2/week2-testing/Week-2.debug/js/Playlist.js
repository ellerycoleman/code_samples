
var Playlist = function(){
  // initialize
  console.log("Playlist() was called with " + arguments.length + " args");
  if(arguments.length === 0)
  {   //do nothing
  }
  else
  {  var i;
     for(i=0; i<arguments.length; i++)
     {   console.log("   args[" + i + "]: " + arguments[i]);
     }
  }
  this.playlist = JSON.parse(sessionStorage.getItem('playlist')) || [];
  this.listenAddSong();

  this.updatePlaylist();

};



Playlist.prototype.listenAddSong = function(){
  console.log("listenAddSong() was called with " + arguments.length + " args");
  if(arguments.length === 0)
  {   //do nothing
  }
  else
  {  var i;
     for(i=0; i<arguments.length; i++)
     {   console.log("   args[" + i + "]: " + arguments[i]);
     }
  }
  var that = this;
  $('#addSongForm').on('submit', function(event){
    that.addSong($('#song').val());
    $('#song').val('');

    return false;
  });
};



Playlist.prototype.addSong = function(song){
  console.log("addSong() was called with " + arguments.length + " args");
  if(arguments.length === 0)
  {   //do nothing
  }
  else
  {  var i;
     for(i=0; i<arguments.length; i++)
     {   console.log("   args[" + i + "]: " + arguments[i]);
     }
  }
  this.playlist.push(song);
  this.updatePlaylist();
};



Playlist.prototype.removeSong = function(index){
  console.log("removeSong() was called with " + arguments.length + " args");
  if(arguments.length === 0)
  {   //do nothing
  }
  else
  {  var i;
     for(i=0; i<arguments.length; i++)
     {   console.log("   args[" + i + "]: " + arguments[i]);
     }
  }
  this.playlist.splice(index, 1);
  this.updatePlaylist();
};



Playlist.prototype.updatePlaylist = function() {
  console.log("updatePlaylist() was called with " + arguments.length + " args");
  if(arguments.length === 0)
  {   //do nothing
  }
  else
  {  var i;
     for(i=0; i<arguments.length; i++)
     {   console.log("   args[" + i + "]: " + arguments[i]);
     }
  }
  sessionStorage.setItem('playlist', JSON.stringify(this.playlist));
  this.updatePlaylistDom();
};



Playlist.prototype.updatePlaylistDom = function(){
  console.log("updatePlaylistDom() was called with " + arguments.length + " args");
  if(arguments.length === 0)
  {   //do nothing
  }
  else
  {  var i;
     for(i=0; i<arguments.length; i++)
     {   console.log("   args[" + i + "]: " + arguments[i]);
     }
  }

  var that = this;
  var playlistDom = this.playlist.map(function(song, index){

    console.log("var playListDom was called with " + arguments.length + " args");
    if(arguments.length === 0)
    {   //do nothing
    }
    else
    {  var i;
       for(i=0; i<arguments.length; i++)
       {   console.log("   args[" + i + "]: " + arguments[i]);
       }
    }
    var removeButton = document.createElement("button");
    removeButton.appendChild(document.createTextNode("remove"));
    $(removeButton).click(function(){
      that.removeSong(index);
    });
    $(removeButton).addClass("btn");

    var li = document.createElement('li');
    li.appendChild(document.createTextNode(song));
    li.appendChild(removeButton);

    return li;
  });

  console.log("var playlistDom was just set with an anonymous function.");
  var i;
  if(playlistDom.length === 0)
  {   console.log("    playlistDom content: NULL");
  }
  for(i=0; i<playlistDom.length; i++)
  {   var liObj= playlistDom[i];
	  console.log("    playlistDom content: ");
	  console.log("    playlistDom[" + i + "]:"
		          + liObj + "-->" + playlistDom[i].innerHTML
		         );
  }
  $('#currentPlaylist').html(playlistDom);
  //console.log($('#currentPlaylist').html(playlistDom));


//  var tmpary=[0,1,2,[17,18,19,20],4,5,{ 'name': 'ellery','id': 1234}];
//  $('#currentPlaylist').html(tmpary);
};





