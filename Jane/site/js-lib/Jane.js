"use strict";var Jane=Object.create(null);Jane.events={DATA_POPULATED:"DATA_POPULATED",DATA_FLUSHED:"DATA_FLUSHED",DATA_CHANGED:"DATA_CHANGED"};Jane.formats={EMPTY:"EMPTY",OBJECT:"OBJECT",OBJECT_AS_PROTOTYPE:"OBJECT_AS_PROTOTYPE"};Jane.dataRefs={};Jane.DataReference=Object.create(null);Jane.DataReference.allowFlushForSubscription=false;Jane.DataReference.Init=function(a){this.subscriptions=[];this.metaData={fields:{},tags:{}};if("name" in a){this["name"]=a.name}if("allowFlushForSubscription" in a){this["allowFlushForSubscription"]=a.allowFlushForSubscription}Jane.dataRefs[this.name]=this;return this};Jane.DataReference.CanSubscribe=function(e){if(Object.getOwnPropertyNames(e).length>0){for(var a in e){if(e.hasOwnProperty(a)){for(var b=0,d=this.subscriptions.length;b<d;++b){var c=this.subscriptions[b];if(a in c.contract){return false}}}}if(this.HasData()&&this.chachedData.readOnly){if(this.allowFlushForSubscription){this.Flush()}else{return false}}}return true};Jane.DataReference.Subscribe=function(f,e,d){if(this.CanSubscribe(d)){for(var a=0,c=this.subscriptions.length;a<c;++a){var b=this.subscriptions[a];if(f==b.target){return false}}this.subscriptions.push({target:f,receiver:e,contract:d});if(this.HasData()){e.apply(f,[Jane.events.DATA_POPULATED,this.cachedData])}return true}return false};Jane.DataReference.SubscribeReadOnly=function(b,a){return this.Subscribe(b,a,{})};Jane.DataReference.Unsubscribe=function(d){for(var a=0,c=this.subscriptions.length;a<c;++a){var b=this.subscriptions[a];if(d==b.target){this.subscriptions.splice(a,1);return}}};Jane.DataReference.PostEvent=function(d){for(var a=0,c=this.subscriptions.length;a<c;++a){var b=this.subscriptions[a];b.receiver.apply(b.target,[this,d])}};Jane.DataReference.HasData=function(){return("cachedData" in this)};Jane.DataReference.GetData=function(){return(this.HasData())?this.cachedData.data:null};Jane.DataReference.GetDataIsReadOnly=function(){if(this.HasData()){return this.cachedData.readOnly}for(var a=0,c=this.subscriptions.length;a<c;++a){var b=this.subscriptions[a];if(Object.getOwnPropertyNames(b.contract).length>0){return false}}return true};Jane.DataReference.GetDataFormat=function(){if(this.HasData()){return this.cachedData.format}return Jane.formats.EMPTY};Jane.DataReference.PopulateDataResponse=function(b,d,c,a){this.cachedData={data:b,readOnly:d,format:c};this.PostEvent(a)};Jane.DataReference.PopulateData=function(){};Jane.DataReference.Populate=function(){if(!this.HasData()){this.PopulateData()}};Jane.DataReference.Flush=function(){if(this.HasData()){delete this.cachedData;this.PostEvent(Jane.events.DATA_FLUSHED)}};Jane.DataReference.Refresh=function(){this.Flush();this.Populate()};Jane.DataReference.HasMetaData=function(){return(Object.getOwnPropertyNames(this.metaData).length>0)};Jane.DataReference.GetMetaData=function(){return this.metaData};Jane.DataReference.AddFieldMetaData=function(h,b,e,c){var g={fieldName:h,displayName:b,type:e};this.metaData.fields[h]=g;for(var d=0,f=c.length;d<f;++d){var a=c[d];if(!(a in this.metaData.tags)){this.metaData.tags=[]}this.metaData.tags[a].push(g)}};Jane.DataReferenceJson=Object.create(Jane.DataReference);Jane.DataReferenceJson.Init=function(a){(Object.getPrototypeOf((Object.getPrototypeOf(this)))).Init.call(this,a);if("url" in a){this["url"]=a.url}if("metaDataUrl" in a){this["metaDataUrl"]=a.metaDataUrl}this.PopulateMetaData();return this};Jane.DataReferenceJson.PopulateMetaData=function(){if(!this.HasMetaData()){var a=this;$.getJSON(this.metaDataUrl,function(b){a.PopulateMetaDataResponse(b)})}};Jane.DataReferenceJson.PopulateMetaDataResponse=function(b){var a=b.columns;for(var c=0,d=a.length;c<d;++c){var e=a[c];this.AddFieldMetaData(e.accessName,e.displayName,e.types[0],e.tags)}if("populateRequested" in this){delete this.populateRequested;this.PopulateData()}};Jane.DataReferenceJson.PopulateData=function(){if(Object.getOwnPropertyNames(this.metaData).length>0){var a=this;$.getJSON(this.url,function(b){a.PopulateDataResponse(b,true,Jane.formats.OBJECT,Jane.events.DATA_POPULATED)})}else{this.populateRequested=true}};Jane.DataReferenceCopy=Object.create(Jane.DataReference);Jane.DataReferenceCopy.Init=function(a){(Object.getPrototypeOf((Object.getPrototypeOf(this)))).Init.call(this,a);this.filter=null;this.select=null;this.transform=null;this.sort=null;if("source" in a){this["source"]=a.source}this.source.SubscribeReadOnly(this,this.ReceiveEvent);return this};Jane.DataReferenceCopy.Validate=function(){if(!this.HasMetaData()){return false}var a=this.GetMetaData();if(this.select!=null){for(var b=0,c=this.select.length;b<c;++b){var d=this.select[b];if(!(d in a)){return false}}}if(this.sort!=null){for(var b=0,c=this.sort.length;b<c;++b){var d=this.sort[b].name;if(!(d in a)){return false}}}return true};Jane.DataReferenceCopy.CopyData=function(l,b){if(this.Validate()){var d=l.data;var o=this.GetDataFormat();var n=this.GetDataIsReadOnly();var s=[];for(var e=0,h=d.length;e<h;++e){var g=d[e];if((this.filter==null)||(this.filter.HandleRecord(g))){if(this.select==null){if(!n){g=Object.create(g);o=Jane.formats.OBJECT_AS_PROTOTYPE}}else{var k={};for(var c=0,m=this.select.length;c<m;++c){var p=this.select[c];k[p]=g[p]}g=k;o=Jane.formats.OBJECT}if(this.transform!=null){g=this.transform.HandleRecord(g)}s.push(g)}}if(this.sort!=null){var q=this;var a=this.GetMetaData();var f=this.sort.length;var r=function(t,j){var y=function(B,z,E,F){switch(E){case"integer":case"double":case"string":var A=Number(B);var i=Number(z);if((A==B.toString())&&(i==z.toString())){return F?(A-i):(i-A)}return F?B.localeCompare(z):z.localeCompare(B);break;case"temporal":var D=new Date(B).valueOf();var C=new Date(z).valueOf();return F?(D-C):(C-D);break}return 0};for(var w=0,x=q.sort.length;w<x;++w){var u=q.sort[w];if(u.name in a){var v=y(t[u.name],j[u.name],a[u.name].type,u.asc);if(v!=0){return v}}}return 0};s.sort(r)}this.PopulateDataResponse({data:s},n,o,b)}};Jane.DataReferenceCopy.ReceiveEvent=function(a,b){switch(b){case Jane.events.DATA_POPULATED:if("populateRequested" in this){delete this.populateRequested;this.CopyData(a.GetData(),b)}break;case Jane.events.DATA_CHANGED:if(this.HasData()){this.CopyData(a.GetData(),b)}break;case Jane.events.DATA_FLUSHED:if(this.HasData()){this.Flush()}break;default:break}};Jane.DataReferenceCopy.PopulateData=function(){var a=this.source.GetData();if(a!=null){this.CopyData(a,Jane.events.DATA_POPULATED)}else{this.populateRequested=true;this.source.Populate()}};Jane.DataReferenceCopy.HasMetaData=function(){return((Object.getPrototypeOf((Object.getPrototypeOf(this)))).HasMetaData.call(this))?true:this.source.HasMetaData()};Jane.DataReferenceCopy.GetMetaData=function(){return((Object.getPrototypeOf((Object.getPrototypeOf(this)))).HasMetaData.call(this))?(Object.getPrototypeOf((Object.getPrototypeOf(this)))).GetMetaData.call(this):this.source.GetMetaData()};