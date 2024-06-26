// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "@openzeppelin/contracts/token/ERC721/ERC721.sol";
import "@openzeppelin/contracts/token/ERC721/extensions/ERC721URIStorage.sol";
import "@openzeppelin/contracts/access/Ownable.sol";

contract MyNFT is ERC721 {
    uint256 private _tokenIds;

    struct CustomData {
        string field1;
        string field2;
        string field3;
        string field4;
    }

    mapping(uint256 => CustomData) private _tokenCustomData;

    constructor() ERC721("MyNFT", "MNFT") {}

    function mintNFT(address recipient, string memory tokenURI, CustomData memory customData) 
        public onlyOwner returns (uint256) 
    {
        _tokenIds++;
        uint256 newItemId = _tokenIds;
        _mint(recipient, newItemId);
        _setTokenURI(newItemId, tokenURI);
        _tokenCustomData[newItemId] = customData;

        return newItemId;
    }

    function getTokenCustomData(uint256 tokenId) 
        public view returns (CustomData memory) 
    {
        require(_exists(tokenId), "Token does not exist");
        return _tokenCustomData[tokenId];
    }
}
