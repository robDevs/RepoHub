# About
RepoHub allows you to browse GitHub users and repositories using the GitHub API. You can view and download releases from repositories as well. 

## Rate-Limiting
By default the API allows 60 requests per hour, per ip address. For light usage this should be plenty. If you need additional api requests please view the section about personal access tokens. 

## Personal Access Tokens
Using a personall access token increases the API request limit to 5000 per hour and is based on the token, not the ip address. 

Using a token will also unlock these additional features:
- Star and Unstar repositories. 
- Submit issues directly to RepoHub on GitHub

## Creating Personal Access Tokens
- visit https://github.com/settings/tokens to create your token
- Select `public_repo` for the scope of the token. 
- Launch RepoHub and press Start on the home screen
- Select Enter Token
- Your token will be encrypted and saved using a key only provided at compile time. 

## Usage without a Personal Access Token
A user name is still required to use RepoHub. It is used to generate your list of followed users and starred repositories. To add your username:
- press start from the home screen
- select Enter UserName

## Credits
### Programming/Design
- robDevs
### Icons/Live Area
- [MotoLegacy](https://github.com/MotoLegacy)
### Testing/Design Feedback
- [Grzybojad](https://github.com/Grzybojad)
- [MotoLegacy](https://github.com/MotoLegacy)
- [nobodywasishere](https://github.com/nobodywasishere)
